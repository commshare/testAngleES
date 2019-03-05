attribute vec2 vPosition;

varying vec2 xy;

void main() {
    xy = vPosition * 1.5;
    gl_Position = vec4(vPosition, 0.0, 1.0);
}
