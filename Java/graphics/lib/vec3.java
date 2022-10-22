package lib;

public class vec3 {
    private double[] data;


    // ----- Constructors:
    
    public vec3() {
        data = new double[3];
    }
    
    public vec3(final double x, final double y, final double z) {
        this();
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }

    public vec3(double v) {
        this(v, v, v);
    }


    // ----- Methods:

    public vec3 copy() {
        return new vec3(data[0], data[1], data[2]);
    }

    public double mag() {
        return Math.sqrt(
                data[0] * data[0] +
                data[1] * data[1] +
                data[2] * data[2]);
    }

    public vec3 add(final vec3 other) {
        data[0] += other.data[0];
        data[1] += other.data[1];
        data[2] += other.data[2];
        return this;
    }

    public vec3 sub(final vec3 other) {
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        data[2] -= other.data[2];
        return this;
    }

    public vec3 add(final double v) {
        data[0] += v;
        data[1] += v;
        data[2] += v;
        return this;
    }

    public vec3 sub(final double v) {
        data[0] -= v;
        data[1] -= v;
        data[2] -= v;
        return this;
    }

    public vec3 mult(final double v) {
        data[0] *= v;
        data[1] *= v;
        data[2] *= v;
        return this;
    }

    public vec3 div(final double v) {
        data[0] /= v;
        data[1] /= v;
        data[2] /= v;
        return this;
    }

    public double dot(final vec3 other) {
        return data[0] * other.data[0]
            + data[1] * other.data[1]
            + data[2] * other.data[2];
    }

    public double distTo(final vec3 other) {
        return dist(this, other);
    }
    
    // ----- Static:

    public static vec3 add(final vec3 v1, final vec3 v2) {
        return new vec3(
            v1.data[0] + v2.data[0],
            v1.data[1] + v2.data[1],
            v1.data[2] + v2.data[2]);
    }
    
    public static vec3 sub(final vec3 v1, final vec3 v2) {
        return new vec3(
            v1.data[0] - v2.data[0],
            v1.data[1] - v2.data[1],
            v1.data[2] - v2.data[2]);
    }
    
    public static vec3 add(final vec3 v, final double f) {
        return new vec3(
            v.data[0] + f,
            v.data[1] + f,
            v.data[2] + f);
    }
        
    public static vec3 sub(final vec3 v, final double f) {
        return new vec3(
            v.data[0] - f,
            v.data[1] - f,
            v.data[2] - f);
    }
                    
    public static vec3 mult(final vec3 v, final double f) {
        return new vec3(
            v.data[0] * f,
            v.data[1] * f,
            v.data[2] * f);
    }
                        
    public static vec3 div(final vec3 v, final double f) {
        return new vec3(
            v.data[0] / f,
            v.data[1] / f,
            v.data[2] / f);
    }
                            
    public static double dot(final vec3 v1, final vec3 v2) {
        return v1.data[0] * v2.data[0]
        + v1.data[1] * v2.data[1]
        + v1.data[2] * v2.data[2];
    }

    public static double dist(final vec3 v1, final vec3 v2) {
        return vec3.sub(v1, v2).mag();
    }
    
    // ----- Member access:
    
    public double getX() {
        return data[0];
    }
    
    public double getY() {
        return data[1];
    }
    
    public double getZ() {
        return data[2];
    }

    public void setX(final double x) {
        data[0] = x;
    }

    public void setY(final double y) {
        data[1] = y;
    }

    public void setZ(final double z) {
        data[2] = z;
    }
}
