Name:     cidrl
Version:  1.0.0
Release:  %autorelease
Summary:  List all IP addresses within a CIDR block.
License:  BSD
URL:      https://github.com/emden-norfolk/cidrl
Source:   https://github.com/emden-norfolk/cidrl/archive/refs/tags/v%{version}.tar.gz

%description
CIDRL lists all IP addresses within a CIDR block (IPv4 and IPv6.) Supports subnetting.

%prep
%autosetup

%build
%configure
%make_build

%install
%make_install

%files

%changelog
%autochangelog
