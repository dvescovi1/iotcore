#! /usr/bin/perl -w
use strict;

my $load_address = 0x40480000;
my $uefi_padded_size = {UEFI_PADDED_SIZE};
my $ivt_size = 0x20;

open(my $out, '>:raw', 'ivt.bin') or die "Unable to open: $!";
print $out pack("V", 0x412000D1); # Signature
print $out pack("V", $load_address); # Load Address (*load_address)
print $out pack("V", 0x0); # Reserved
print $out pack("V", 0x0); # DCD pointer
print $out pack("V", 0x0); # Boot Data
print $out pack("V", $load_address + $uefi_padded_size); # Self Pointer (*ivt)
print $out pack("V", $load_address + $uefi_padded_size + $ivt_size); # CSF Pointer (*csf)
print $out pack("V", 0x0); # Reserved
close($out);
