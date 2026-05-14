#version 430 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform uint postfx;

// Bit flags
const uint FX_INVERT    = 1u << 0u;
const uint FX_GRAYSCALE = 1u << 1u;
const uint FX_SHARPEN   = 1u << 2u;
const uint FX_BLUR      = 1u << 3u;
const uint FX_EDGE      = 1u << 4u;

const float offset = 1.0 / 300.0;

vec3 applyPointEffects(vec3 color)
{
        // Inversion
        if ((postfx & FX_INVERT) != 0u)
        {
                color = 1.0 - color;
        }

        // Grayscale
        if ((postfx & FX_GRAYSCALE) != 0u)
        {
                float gray = dot(color, vec3(0.2126, 0.7152, 0.0722));
                color = vec3(gray);
        }

        return color;
}

vec3 applyKernel(vec3 samples[9], float kernel[9])
{
        vec3 result = vec3(0.0);

        for (int i = 0; i < 9; ++i)
        {
                result += samples[i] * kernel[i];
        }

        return result;
}

void main()
{
        // 3x3 offsets around the current pixel.
        vec2 offsets[9] = vec2[](
                vec2(-offset,  offset), // top-left
                vec2( 0.0,     offset), // top-center
                vec2( offset,  offset), // top-right

                vec2(-offset,  0.0),    // center-left
                vec2( 0.0,     0.0),    // center
                vec2( offset,  0.0),    // center-right

                vec2(-offset, -offset), // bottom-left
                vec2( 0.0,    -offset), // bottom-center
                vec2( offset, -offset)  // bottom-right
        );

        // Sample the 3x3 neighborhood and apply point-wise effects to every sample.
        // This ensures invert/grayscale affect the data used by the kernel.
        vec3 samples[9];

        for (int i = 0; i < 9; ++i)
        {
                vec3 sampleColor =
                texture(screenTexture, TexCoords + offsets[i]).rgb;

                sampleColor = applyPointEffects(sampleColor);

                samples[i] = sampleColor;
        }

        // Start with the center sample after point-wise processing.
        vec3 color = samples[4];

        // -------------------------------------------------------------------------
        // Apply at most one kernel effect.
        //
        // Multiple convolution kernels cannot be correctly chained in a single pass,
        // because each kernel needs access to neighboring pixels AFTER the previous
        // kernel has modified them.
        //
        // Priority:
        //   Edge Detection > Sharpen > Blur
        // -------------------------------------------------------------------------

        if ((postfx & FX_EDGE) != 0u)
        {
                float kernel[9] = float[](
                1.0,  1.0,  1.0,
                1.0, -8.0,  1.0,
                1.0,  1.0,  1.0
                );

                color = applyKernel(samples, kernel);
        }
        else if ((postfx & FX_SHARPEN) != 0u)
        {
                float kernel[9] = float[](
                -1.0, -1.0, -1.0,
                -1.0,  9.0, -1.0,
                -1.0, -1.0, -1.0
                );

                color = applyKernel(samples, kernel);
        }
        else if ((postfx & FX_BLUR) != 0u)
        {
                float kernel[9] = float[](
                1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
                2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
                1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
                );

                color = applyKernel(samples, kernel);
        }

        FragColor = vec4(color, 1.0);
}