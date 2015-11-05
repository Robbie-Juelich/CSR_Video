#!/usr/bin/perl
use strict;
use warnings;
use SVG;


if (scalar(@ARGV) > 0  && $ARGV[0] eq "clean") {
	`rm -rf tmp`;
	`rm -rf *~`;
	`rm -rf tw2828*.c`;
	`rm -rf *ch.yuv`;
	`rm -rf *ch.svg`;
	`rm -rf *ch.png`;
	exit;
}

my (%param_hash, %chars_hash);
my $tmp_dir = "tmp";

my $char_w = 20;
my $char_h = 28;
my $y_inc = 40;
my $x_inc = 600;
my $deg = chr(0xb0);
my $font = "Helvetica, Arial, sans-serif";


###########################################
## to generate all char data
###########################################

print "Generating chars data into tmp\n";
init_chars_hash();
open CHARS, ">tw2828_chars_data.c" or die $!;
my ($key, $value);
foreach $key (sort keys%chars_hash){
    $value = $chars_hash{$key};
    out_chars($key, $value);
}

print CHARS "unsigned char* chars_data[] = {\n";
for (my $i = 0; $i <= 255; ++$i) {
    if( exists $chars_hash{$i}) {
        print CHARS "\tchar_data\_$chars_hash{$i},\n";
    }
    else {
        print CHARS "\tchar_data\_null,\n";
    }
}
print CHARS "};\n";
close CHARS;

#########################################
## to generate all pictures
#########################################
my %hash = (
	'4ch' => 'param_4ch.txt',
	'6ch' => 'param_6ch.txt',
	'9ch' => 'param_9ch.txt',
	'1ch' => 'param_1ch.txt'
);

while (my($k,$v) = each %hash) {
    print "Generating logo data $k\n";
    read_param($v);   #### read parameters into param_hash
	my $svg = SVG->new(width=>$param_hash{"W"} , height=> $param_hash{"H"});
    draw_logo($svg);  #### draw the csr logo
    draw_logo_chars($svg);  ### draw all chars

	my ($width, $height) = ($param_hash{'W'}, $param_hash{'H'});
    my $i = 0x0;
    my @array = output_logo($svg, $k);

    open OUT, ">tw2828_logodata$k.c"    or die $!;
    printf OUT  "\nunsigned char logodata_%s[] = {\n", $k;
    printf OUT  "\t0x00, 0x00, 0x%02x, 0x%02x, 0x%02x, 0x%02x, ".
                "0x12, 0x00, 0x00, 0x61,\n",
                $width & 0xff, ($width >> 8) & 0xff,
                $height & 0xff, ($height >> 0x8) & 0xff;
    printf OUT  "\t0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,";
    for($i = 0x0; $i < scalar(@array); ++$i) {
        if ($i%10 == 0x0) {
            printf OUT "\n\t";
        }
        printf OUT "0x%02x, ", $array[$i];
    }
    printf OUT "\n};\n\n";
    close OUT;

}


open  OUT, ">tw2828_logodata.c" or die $!;
print OUT "#include \"tw2828_logo.h\"\n";

while (($key, $value) = each %hash) {
    print OUT "#include \"tw2828_logodata$key.c\"\n";
}

print OUT "\n";
print OUT "#define LOGO_4ch  0x0\n";
print OUT "#define LOGO_6ch  0x1\n";
print OUT "#define LOGO_9ch  0x2\n";
print OUT "#define LOGO_1ch  0x3\n";
print OUT "\n";
print OUT "_Logo_Info *cur_logo;\n";
print OUT "_Logo_Info logo_info [] = {\n";

foreach $key (sort keys%hash){
	read_param($hash{$key});
    print OUT "\t[LOGO_$key] = {\n";
    print OUT "\t\t.logo_data = logodata_$key,\n";
    print OUT "\t\t.x0 = ", $param_hash{"X0"}, ",\n";
    print OUT "\t\t.y0 = ", $param_hash{"Y0"}, ",\n";
    print OUT "\t\t.width = ", $param_hash{"W"}, ",\n";
    print OUT "\t\t.height = ", $param_hash{"H"}, ",\n";
    print OUT "\t\t.entries[0] = {\n";
    print OUT "\t\t\t.x = ", $param_hash{"DIGIT_X"}, ",\n";
    print OUT "\t\t\t.y = ", $param_hash{"DIGIT_Y"}, ",\n";
	print OUT "\t\t\t.header = NULL,\n";
	print OUT "\t\t\t.data = NULL,\n";
    print OUT "\t\t},\n";
    print OUT "\t\t.entries[1] = {\n";
	if($param_hash{'CH'} == 1) {
    	print OUT "\t\t\t.x = ", $param_hash{"DIGIT_X"} + $x_inc, ",\n";
    	print OUT "\t\t\t.y = ", $param_hash{"DIGIT_Y"} , ",\n";
	} else {
    	print OUT "\t\t\t.x = ", $param_hash{"DIGIT_X"} , ",\n";
    	print OUT "\t\t\t.y = ", $param_hash{"DIGIT_Y"} + $y_inc, ",\n";
	}
	print OUT "\t\t\t.header = NULL,\n";
	print OUT "\t\t\t.data = NULL,\n";
    print OUT "\t\t},\n";
    print OUT "\t},\n";
}
printf OUT "};\n\n";

print OUT "#define HEADER_SIZE\t20\n";
print OUT "#define CHAR_WIDTH\t", $char_w, "\n";
print OUT "#define CHAR_HEIGHT\t", $char_h, "\n";
print OUT "#define CHAR_SIZE\t", $char_h * $char_w * 2, "\n";
print OUT "\n";

print OUT "unsigned char yuvdata_header[HEADER_SIZE] = { \n";
print OUT "\t0x00, 0x00, CHAR_WIDTH & 0xff, (CHAR_WIDTH  >> 8) & 0xff,\n";
print OUT "\tCHAR_HEIGHT & 0xff, (CHAR_HEIGHT >>8) & 0xff, 0x12, 0x00,\n";
print OUT "\t0x00, 0x61,";
print OUT "\t0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n";
print OUT "};\n\n";

close OUT;


######################################################
## all functions
######################################################

sub read_param
{
    my ($input) = @_;
	undef %param_hash;
    open INPUT, $input or die $!;
    while (<INPUT>) {
        my($key, $value) = split(/[\s]+/, $_);
        $param_hash{$key} = $value;
    }
    close INPUT;
}

sub draw_logo{
	my $svg = shift;
	$svg->rectangle(x => 0, y => 0, width => $param_hash{"W"}, 
		height => $param_hash{"H"}, fill => "black", stroke => 'white');
	my $width = $param_hash{"LOGO_W"};
	my $height = $param_hash{"LOGO_H"};
	$svg->image(x => $param_hash{'W'} - $width - 5, 
		y => $param_hash{'H'} - $height - 5,
    	width => $width, height => $height,
   		'-href' => $param_hash{"LOGO"});
}

sub draw_logo_chars {
	my $svg =  shift;
	my $x = $param_hash{'DIGIT_X'};
	my $y = $param_hash{'DIGIT_Y'};
	my @array_gps = ('0', '0', '0', $deg, '0', '0', '\'', '0', '0', '"', 'E',
                ' ', ' ', ' ',
                '0', '0', '0', $deg, '0', '0', '\'', '0', '0', '"', 'N');
    my @array_mil = ('0', '0', '0', '0', '.', '0', '0', '0', 'K', 'm',
                ' ', ' ', ' ', ' ',
                '0', '0', '0', 'K', 'm', '/', 'h');
	
	foreach(@array_gps) {
		$svg->image(x => $x, y => $y, width => $char_w, height => $char_h,
			'-href' => ("$tmp_dir/" . $chars_hash{ord($_)} . ".svg"));
		$x = $x + $char_w;
	}
	$x = $param_hash{'DIGIT_X'};
	if($param_hash{'CH'} == 1) {
		$x = $x + $x_inc;
	}
	else {
		$y = $y + $y_inc;
	}

	foreach(@array_mil) {
		$svg->image(x => $x, y => $y, width => $char_w, height => $char_h,
			'-href' => "$tmp_dir/" . $chars_hash{ord($_)} . ".svg");
		$x = $x + $char_w;
	}
}

sub output_logo {
    my ($svg, $out_file) = @_;
    my $out;
    $out = $svg->xmlify;
    open SVGFILE, ">$out_file.svg";
    print SVGFILE $out;
    close SVGFILE;
    `convert $out_file.svg $out_file.png`;
    `convert $out_file.svg pal:$out_file.yuv`;

    my $buf;
    my $size = (stat "$out_file.yuv")[7];
    open YUV, "$out_file.yuv" or die $!;
    binmode(YUV);
    read(YUV, $buf, $size);
    my @array = unpack("C*", $buf);
    close YUV;
    return @array;
}




sub init_chars_hash {
    my $i = 0x0;
    for(; $i <= 9; ++$i){
        $chars_hash{ord("$i")} = "$i";
    }
    $chars_hash{ord($deg)} = 'deg';
    $chars_hash{ord("\"")}= 'mil';
    $chars_hash{ord("\'")} = 'sec';
    $chars_hash{ord("K")} = 'K';
    $chars_hash{ord("k")} = 'k';
    $chars_hash{ord("M")} = 'M';
    $chars_hash{ord("m")} = 'm';
    $chars_hash{ord("/")} = "sla";
    $chars_hash{ord("H")} = 'H';
    $chars_hash{ord("h")} = 'h';
    $chars_hash{ord("N")} = 'N';
    $chars_hash{ord("S")} = 'S';
    $chars_hash{ord("W")} = 'W';
    $chars_hash{ord("E")} = 'E';
    $chars_hash{ord(".")} = 'dot';
    $chars_hash{ord(" ")} = 'null';
}


sub draw_char {
    my ($svg, $ch, $x, $y) = @_;
    $svg->text(x => $x , y => $y, 'fill' =>"black",
        'font-family' => $font,"text-anchor"=>"middle",
        'font-weight' => 'bold',
		'fill' => 'white',
        'font-size' =>"28")->cdata(chr($ch));
}

sub out_chars {

    my ($ch ,$out_file) = @_;
    my ($svg, $out);
	my ($w, $h) = ($char_w, $char_h);
    $svg = SVG->new(width => $w, height => $h);
	$svg->rectangle(x=> 0, y=>0, width=> $w, height=>$h, fill=>"black", "stroke" => "black");
    draw_char($svg, $ch, $w / 2, $h - 4);
    $out = $svg->xmlify;
	`mkdir -p $tmp_dir`;
    open SVGFILE, ">$tmp_dir/$out_file.svg";
    print SVGFILE $out;
    close SVGFILE;

    `inkscape $tmp_dir/$out_file.svg -e $tmp_dir/$out_file.png -w 20 -h 28`;
    `convert $tmp_dir/$out_file.png pal:$tmp_dir/$out_file.yuv`;
    `convert $tmp_dir/$out_file.png $tmp_dir/$out_file.bmp`;

    my $buf;
    my $size = (stat "$tmp_dir/$out_file.yuv")[7];
    open YUV, "$tmp_dir/$out_file.yuv" or die $!;
    binmode(YUV);
    read(YUV, $buf, $size);
    my @array = unpack("C*", $buf);
    print CHARS "\nunsigned char char_data\_$out_file\[\] = {";
    for(my $i = 0; $i < $size; $i = $i + 1) {
        if ($i%10 == 0x0) {
            print CHARS "\n\t";
        }
        printf CHARS "0x%02x, ", $array[$i];
    }
    print CHARS "\n};\n";
    close YUV;
}

