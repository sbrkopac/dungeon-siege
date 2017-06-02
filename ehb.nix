# nix-shell -p bison cmake flex gcc libxdg_basedir mesa openscenegraph pkgconfig --run "kdevelop -s ehb"

with import <nixpkgs> {};

stdenv.mkDerivation {
    name = "ehb-dev"; # Probably put a more meaningful name here
    buildInputs = [ bison cmake flex gcc libxdg_basedir mesa openscenegraph pkgconfig ];
}
