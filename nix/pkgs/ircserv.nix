{
  lib,
  stdenv,
  libcxxClang,
}:
stdenv.mkDerivation {
  name = "ft-irc";
  src = ../../ft-irc;
  nativeBuildInputs = [
    libcxxClang
  ];
  installPhase = ''
    mkdir -p $out/bin
    cp ircserv $out/bin/ircserv
  '';
  meta = with lib; {
    description = "A simple IRC Server written in C++ (aka WhatsApp 2).";
    license = licenses.agpl3Only;
  };
}
