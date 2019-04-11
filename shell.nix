let
  hostPkgs = import <nixpkgs> {};
  nixpkgs = (hostPkgs.fetchFromGitHub {
    owner = "NixOS";
    repo = "nixpkgs-channels";
    # SHA for latest commit on 2019-03-10 for the nixos-19.03 branch
    rev = "aea9130d2fe8bbf39ed6c9115de2516f83d7e298";
    sha256 = "1w1dg9ankgi59r2mh0jilccz5c4gv30a6q1k6kv2sn8vfjazwp9k";
  });
in
  with import nixpkgs {
    overlays = [
      (import ~/.config/nixpkgs/overlays/default.nix)
      (self: super: {
         python3 = super.python3.override {
           packageOverrides = py-self: py-super: {
             python-language-server = py-super.python-language-server.override {
               providers = [
                 "rope"
                 "pyflakes"
                 "mccabe"
                 "pycodestyle"
                 "pydocstyle"
                 "yapf"
               ];
             };
           };
         };
      })
    ];
  };

  stdenv.mkDerivation {
    name = "linalg";
    buildInputs = [
      cmake
      gcc
      gdb
      hdf5
      mkl
      ninja-kitware
      pybind11
      python3Packages.epc
      python3Packages.importmagic
      python3Packages.isort
      python3Packages.jupyterlab
      python3Packages.mypy
      python3Packages.numpy
      python3Packages.poetry
      python3Packages.pyls-isort
      python3Packages.pyls-mypy
      python3Packages.python-language-server
      travis
      valgrind
      zlib
    ];
    hardeningDisable = [ "all" ];
    src = null;
    shellHook = ''
    SOURCE_DATE_EPOCH=$(date +%s)
    '';
  }
