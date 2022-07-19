{ pkgs ? import <nixpkgs> {} }:

let
  pyversion = pkgs.python3;
  python-with-my-packages = pyversion.withPackages (p: with p; [
    attrs
    black
    pyzmq
    ipython
    python-lsp-server
    pip
  ]);
in
pkgs.mkShell {
  name = "wahwee-shell-project";
  buildInputs = with pkgs; [
     gcc
     glibc
     cmake
     cppzmq
     zmqpp
     ninja
     libyamlcpp
     python-with-my-packages
  ];
  
  shellHook = ''
   # Some bash command and export some env vars.
   echo "Starting Wawhee builder shell";
   LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib/
  '';
}
