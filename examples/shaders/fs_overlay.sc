$input v_texture_uv, v_color0

#include <shader_include.sh>

SAMPLER2D(s_texture, 0);

uniform vec4 u_alpha;

void main() {
  gl_FragColor = texture2D(s_texture, v_texture_uv) * v_color0 * u_alpha;
}
