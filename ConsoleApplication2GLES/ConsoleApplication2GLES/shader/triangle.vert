attribute vec4 vPosition;
attribute vec4 vColor;

varying vec4 fColor;

void main() {
    fColor = vec4(vColor.rgb, 1.0);
    gl_Position = vPosition;
}
