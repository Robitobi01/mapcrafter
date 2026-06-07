{ pkgs ? import <nixpkgs> {}
, lib ? pkgs.lib
, stdenv ? pkgs.stdenv
, cmake ? pkgs.cmake
, boost ? pkgs.boost
, libpng ? pkgs.libpng
, libjpeg ? pkgs.libjpeg
}:

stdenv.mkDerivation {
  pname = "mapcrafter";
  version = "unstable";

  src = lib.cleanSourceWith {
    src = lib.cleanSource ./.;
    filter = name: type:
      !(type == "directory" && baseNameOf name == "build");
  };

  nativeBuildInputs = [ cmake ];
  buildInputs = [ boost libpng libjpeg ];

  cmakeFlags = [ "-DOPT_SKIP_TESTS=ON" ];
}
