{
  inputs = {
    nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/*.tar.gz";
    ft-nix = {
      url = "github:vinicius507/42-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = {
    self,
    nixpkgs,
    ft-nix,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
      overlays = [self.overlays.ircserv];
    };
  in {
    packages.${system} = {
      default = self.packages.${system}.ircserv;
      ircserv = import ./nix/pkgs/ircserv.nix {
        inherit (pkgs) lib;
        inherit (pkgs.llvmPackages_12) stdenv libcxxClang;
      };
    };
    overlays = {
      default = self.overlays.ircserv;
      ircserv = final: _: {
        ircserv = self.packages.${final.system}.ircserv;
      };
    };
    devShells.${system}.default = ft-nix.lib.mkShell {
      packages = with pkgs; [
        bear
        clang-tools_12
        gnumake
        llvmPackages_12.libcxxClang
        valgrind
      ];
    };
  };
}
