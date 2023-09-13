{
  stdenv,
  libcxxClang,
  minunit,
}:
stdenv.mkDerivation {
  name = "tests";
  src = ../../ft-irc;
  hardeningDisable = ["all"];
  nativeBuildInputs = [
    libcxxClang
    minunit
  ];
  installPhase = ''
    mkdir -p $out/bin/run_tests
    cp run_tests $out/bin/run_tests
  '';
  checkPhase = ''
    make tests
  '';
  doCheck = true;
  dontBuild = true;
}
