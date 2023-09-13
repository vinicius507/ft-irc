{
  lib,
  stdenv,
  fetchFromGitHub,
}:
stdenv.mkDerivation {
  name = "minunit";
  src = fetchFromGitHub {
    owner = "siu";
    repo = "minunit";
    rev = "a0586569bd8abfb7a37390d670484c201e9173bf";
    hash = "sha256-E6+ySj1+kjWBGSHGB9wsIgbzIroQvpcGdIIFDZmakPw=";
  };
  installPhase = ''
    mkdir -p $out/include
    cp minunit.h $out/include/minunit.h
  '';
  meta = with lib; {
    description = "Minimal unit testing framework for C";
    homepage = "https://github.com/siu/minunit";
    license = licenses.mit;
    platforms = platforms.all;
  };
}
