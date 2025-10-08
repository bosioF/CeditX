Name:           CeditX
Version:        1.0.0
Release:        1%{?dist}
Summary:        Lightweight hex editor written in C

License:        MIT
URL:            https://github.com/bosioF/CeditX
Source0:        %{url}/archive/refs/heads/main.tar.gz#/CeditX-%{version}.tar.gz

BuildRequires:  gcc, make
Requires:       glibc

%description
CeditX è un editor esadecimale leggero scritto in C, con interfaccia a riga di comando.
Permette di visualizzare, modificare e salvare file binari con semplicità.

%prep
%autosetup -n CeditX-main

%build
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_bindir}
install -m 0755 CeditX %{buildroot}%{_bindir}/CeditX

%files
%license LICENSE
%doc README.md
%{_bindir}/CeditX

%changelog
* Wed Oct 08 2025 Federico Bosio - 1.0.0-1
- Initial Copr build