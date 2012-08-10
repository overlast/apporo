package Apporo;
use strict;
use warnings;
our $VERSION = '0.01';
our @ISA;

eval {
    require XSLoader;
    XSLoader::load(__PACKAGE__, $VERSION);
    1;
} or do {
    require DynaLoader;
    push @ISA, 'DynaLoader';
    __PACKAGE__->bootstrap($VERSION);
};

1;
__END__

=head1 NAME

Apporo -

=head1 SYNOPSIS

  use Apporo;

=head1 DESCRIPTION

Apporo is

=head1 AUTHOR

Toshinori Satou E<lt>overlasting {at} gmail.comE<gt>

=head1 SEE ALSO

=head1 LICENSE

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself.

=cut
