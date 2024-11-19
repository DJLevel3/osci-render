std::string flippedTexturedVertexShader = R"(

attribute vec2 aPos;
varying vec2 vTexCoord;
uniform float uResizeForCanvas;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vTexCoord = (0.5 * vec2(aPos.x, -aPos.y) + 0.5) * uResizeForCanvas;
}

)";
