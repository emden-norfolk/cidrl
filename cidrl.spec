Name:     cidrl
Version:  1.0.2
Release:  %autorelease
Summary:  List all IP addresses within a CIDR block.
License:  BSD
URL:      https://github.com/emden-norfolk/cidrl
Source:   https://github.com/emden-norfolk/cidrl/archive/refs/tags/v%{version}.tar.gz
BuildRequires:  bash
BuildRequires:  coreutils
BuildRequires:  gcc
BuildRequires:  glibc-common
BuildRequires:  make

%description
CIDRL lists all IP addresses within a CIDR block (IPv4 and IPv6.) Supports subnetting.

%prep
%autosetup

%build
%configure --enable-nls --disable-rpath
%make_build

%install
%make_install

%files
%{_bindir}/cidrl
%{_bindir}/cidrl6
%doc README.md

%changelog
%autochangelog
