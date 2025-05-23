Name		: ethtool
Version		: 6.14
Release		: 1
Group		: Utilities

Summary		: Settings tool for Ethernet and other network devices

License		: GPL
URL		: https://ftp.kernel.org/pub/software/network/ethtool/

Buildroot	: %{_tmppath}/%{name}-%{version}
Source		: %{name}-%{version}.tar.gz


%description
This utility allows querying and changing settings such as speed,
port, auto-negotiation, PCI locations and checksum offload on many
network devices, especially Ethernet devices.

%prep
%setup -q


%build
CFLAGS="${RPM_OPT_FLAGS}" ./configure --prefix=%{_prefix} --mandir=%{_mandir}
make


%install
make install DESTDIR=${RPM_BUILD_ROOT}


%files
%defattr(-,root,root)
%{_sbindir}/ethtool
%{_mandir}/man8/ethtool.8*
%{_datadir}/bash-completion/completions/ethtool
%doc AUTHORS COPYING NEWS README


%changelog
