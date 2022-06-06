{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "wahwee-shell-project";
  buildInputs = with pkgs; [
     gcc
#     clang
     cmake
     cppzmq
     zmqpp
     ninja
     libyamlcpp
  ];
  
  shellHook = ''
   # Some bash command and export some env vars.
   echo "Starting Wawhee builder shell";
  '';
}
