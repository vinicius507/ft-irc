{
  inputs = {
    nixpkgs.url = "nixpkgs";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
      overlays = builtins.attrValues self.overlays;
    };
  in {
    checks.${system} = {
      tests = import ./nix/checks/tests.nix {
        inherit (pkgs) minunit;
        inherit (pkgs.llvmPackages_12) stdenv libcxxClang;
      };
    };
    packages.${system} = {
      default = self.packages.${system}.ircserv;
      ircserv = import ./nix/pkgs/ircserv.nix {
        inherit (pkgs) lib;
        inherit (pkgs.llvmPackages_12) stdenv libcxxClang;
      };
      minunit = import ./nix/pkgs/minunit.nix {
        inherit (pkgs) lib fetchFromGitHub;
        stdenv = pkgs.stdenvNoCC;
      };
    };
    overlays = {
      default = self.overlays.ircserv;
      devshell = final: prev: {
        mkShell = prev.mkShell.override {
          inherit (final.llvmPackages_12) stdenv;
        };
      };
      ircserv = final: _: {
        ircserv = self.packages.${final.system}.ircserv;
      };
      minunit = final: _: {
        minunit = self.packages.${final.system}.minunit;
      };
    };
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        bear
        clang-tools_12
        gnumake
        llvmPackages_12.libcxxClang
        minunit
        pre-commit
        valgrind
      ];
    };
  };
}
