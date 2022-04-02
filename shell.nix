{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
  buildInputs = [
    clang
    gcc

    cling
    clang-tools
    valgrind
    cgdb

    gnumake
  ];
}
