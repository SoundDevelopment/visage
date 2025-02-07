$input a_position, a_texcoord0, a_texcoord1, a_texcoord2, a_color0, a_color1, a_color2
$output v_texture_uv, v_color0, v_color1

#include <shader_include.sh>

uniform vec4 u_color_mult;
uniform vec4 u_bounds;
uniform vec4 u_atlas_scale;
uniform vec4 u_origin_flip;

void main() {
  vec2 min = a_texcoord2.xy;
  vec2 max = a_texcoord2.zw;
  vec2 clamped = clamp(a_position.xy, min, max);
  vec2 delta = clamped - a_position.xy;

  vec2 flip = vec2(1.0, u_origin_flip.x);
  v_texture_uv = flip * ((a_texcoord1.xy * flip + delta) * u_atlas_scale.xy);
  vec2 adjusted_position = clamped * u_bounds.xy + u_bounds.zw;
  gl_Position = vec4(adjusted_position, 0.5, 1.0);
  v_color0 = a_color0 * a_color2.x * u_color_mult.x;
  v_color0.a = a_color0.a;
  v_color1 = a_color1 * a_color2.x * u_color_mult.x;
  v_color1.a = a_color1.a;
}
