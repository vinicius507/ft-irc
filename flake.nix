{
  inputs = {
    nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/*.tar.gz";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    allSystems = [
      "x86_64-linux"
      "aarch64-linux"
    ];

    mkShellOverlay = final: prev: {
      mkShell = prev.mkShell.override {
        inherit (final.llvmPackages_12) stdenv;
      };
    };

    forEachSystem = f:
      nixpkgs.lib.genAttrs allSystems (system:
        f {
          pkgs = import nixpkgs {
            inherit system;
            overlays = [
              self.overlays.ircserv
              mkShellOverlay
            ];
          };
        });
  in {
    packages = forEachSystem ({pkgs}: {
      default = self.packages.${pkgs.system}.ircserv;
      ircserv = import ./nix/pkgs/ircserv.nix {
        inherit (pkgs) lib;
        inherit (pkgs.llvmPackages_12) stdenv libcxxClang;
      };
    });
    overlays = {
      default = self.overlays.ircserv;
      ircserv = final: _: {
        ircserv = self.packages.${final.system}.ircserv;
      };
    };
    devShells = forEachSystem ({pkgs}: {
      default = pkgs.mkShell {
        packages = with pkgs; [
          bear
          clang-tools_12
          gnumake
          llvmPackages_12.libcxxClang
          valgrind
        ];
      };
    });
  };
}
