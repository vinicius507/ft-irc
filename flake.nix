{
  inputs = {
    nixpkgs.url = "nixpkgs";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    allSystems = [
      "x86_64-linux"
      "aarch64-linux"
    ];

    forEachSystem = f:
      nixpkgs.lib.genAttrs allSystems (system:
        f {
          pkgs = nixpkgs.legacyPackages.${system};
        });
  in {
    packages = forEachSystem ({pkgs}: {
      default = self.packages.${pkgs.system}.ircserv;
      ircserv = pkgs.stdenv.mkDerivation {
        name = "ft-irc";
        src = ./ft-irc;
        nativeBuildInputs = with pkgs; [
          llvmPackages_12.libcxxClang
        ];
        installPhase = ''
          mkdir -p $out/bin
          cp ircserv $out/bin/ircserv
        '';
        meta = {
          description = "A simple IRC Server written in C++ (aka WhatsApp 2).";
        };
      };
    });
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
