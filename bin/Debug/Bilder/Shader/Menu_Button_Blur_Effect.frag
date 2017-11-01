uniform sampler2D texture;
uniform float blur;

void main()
{
    vec2 offx = vec2(0.0f, blur);
    vec2 offy = vec2(blur, 0.0f);

    float factor = 1.0 / (blur + 0.001f);
    vec2 pos = floor(gl_TexCoord[0].xy * factor + 0.5f) / factor;

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy)               * 4.0f +
                 texture2D(texture, gl_TexCoord[0].xy - offx)        * 2.0f +
                 texture2D(texture, gl_TexCoord[0].xy + offx)        * 2.0f +
                 texture2D(texture, gl_TexCoord[0].xy - offy)        * 2.0f +
                 texture2D(texture, gl_TexCoord[0].xy + offy)        * 2.0f +
                 texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1.0f +
                 texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1.0f +
                 texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1.0f +
                 texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1.0f;

    gl_FragColor = gl_Color * (pixel / 10.0f) * texture2D(texture, pos);
}
