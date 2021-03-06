attribute vec4 a_position;
attribute vec2 a_normal;
attribute vec2 a_colorTexCoords;

uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 pivotTransform;

varying vec2 v_colorTexCoords;

void main()
{
  vec4 pos = vec4(a_position.xyz, 1) * modelView;

  float normalLen = length(a_normal);
  vec4 n = normalize(vec4(a_position.xy + a_normal * kShapeCoordScalar, 0, 0) * modelView);
  vec4 norm = n * normalLen;

  vec4 shiftedPos = norm + pos;
  gl_Position = applyPivotTransform(shiftedPos * projection, pivotTransform, 0.0);
  v_colorTexCoords = a_colorTexCoords;
}
