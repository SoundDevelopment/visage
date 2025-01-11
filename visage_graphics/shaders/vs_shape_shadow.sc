$input a_position, a_texcoord0, a_texcoord1, a_texcoord2, a_color0, a_color1
$output v_coordinates, v_dimensions, v_color0, v_shader_values

#include <shader_include.sh>

uniform vec4 u_color_mult;
uniform vec4 u_bounds;

void main() {
  vec2 min = a_texcoord1.xy;
  vec2 max = a_texcoord1.zw;
  vec2 clamped = clamp(a_position.xy + a_texcoord0.xy * 0.5, min, max);
  vec2 delta = clamped - a_position.xy;

  v_dimensions = a_texcoord0.zw;
  v_coordinates = a_texcoord0.xy + 2.0 * delta / v_dimensions;
  vec2 adjusted_position = (clamped + v_coordinates * 0.5) * u_bounds.xy + u_bounds.zw;
  gl_Position = vec4(adjusted_position, 0.5, 1.0);
  v_shader_values = a_texcoord2;
  v_coordinates = v_coordinates * (1.0f + 4.0f * a_texcoord2.y / v_dimensions);
  v_color0 = a_color0 * a_color1.x * u_color_mult.x;
  v_color0.a = a_color0.a;
}
