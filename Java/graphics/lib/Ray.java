package lib;

import lib.vec3;

public class Ray {
    public vec3 orig, dir;

    public Ray(final vec3 orig, final vec3 dir) {
        this.orig = orig;
        this.dir = dir;
    }

    public vec3 at(final double t) {
        return vec3.add(orig, vec3.mult(dir, t));
    }
}
