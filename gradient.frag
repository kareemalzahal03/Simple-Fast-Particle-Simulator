uniform vec4 c;  // The color at the center
uniform vec2 p;       // The position of the circle's center
uniform float r;      // The radius of the circle

void main() {
    vec2 pos = gl_FragCoord.xy;
    float dist = distance(p, pos);

    float factor = 1.0 - clamp(dist / r, 0.0, 1.0);

    gl_FragColor = vec4(c.rgb, factor * factor * c.a);
}
