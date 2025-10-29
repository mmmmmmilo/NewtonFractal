//Calculating the closest root for every pixel
export void computeNewton(uniform int n, uniform int size, uniform uint8* uniform image) {
    uniform double pi = 3.1415926535897932;
    uniform double scale = 2.4;
    uniform int max_iterations = 128;
    uniform double to_root = 0.00001;

    //Creating arrays of real and imaginary roots
    uniform double roots_r[256];
    uniform double roots_i[256];
    foreach (i = 0 ... n) {
        roots_r[i] = cos(pi * 2.0 * (double)i * 1.0 / (double)n);
        roots_i[i] = sin(pi * 2.0 * (double)i * 1.0 / (double)n);
    }

    //Iterating over every pixel in parallel
    foreach (py = 0 ... size, px = 0 ... size) {
        double zr = -scale + ((double) px + 0.5) * 2 * scale / (double)size;
        double zi = -scale + ((double) py + 0.5) * 2 * scale / (double)size;

        //Getting close to some root
        int iterations;
        for (iterations = 0; iterations < max_iterations; iterations++) {
            double f_zr_p = zr;
            double f_zi_p = zi;

            //f_z_p = z^(n-1)
            for (int i = 1; i < n - 1; i++) {
                double f_zr_p2 = f_zr_p * zr - f_zi_p * zi;
                f_zi_p = f_zr_p * zi + f_zi_p * zr;
                f_zr_p = f_zr_p2;
            }

            //f_z = z^n - 1
            double f_zr = f_zr_p * zr - f_zi_p * zi - 1.0;
            double f_zi = f_zr_p * zi + f_zi_p * zr;

            //Calculating the distance to the root and checking if it was found
            double root_length = sqrt(f_zr * f_zr + f_zi * f_zi);
            if (root_length < to_root) {
                break;
            }

            //f_z_p = f'(z) - Calculating the derivative
            f_zr_p *= (double)n;
            f_zi_p *= (double)n;

            //Stopping this iteration before dividing by 0 and changing z a little
            double f_z_p_length = f_zr_p * f_zr_p + f_zi_p * f_zi_p;
            if (f_z_p_length < 0.0000000000000001) {
                zr += 0.000001;
                zi += 0.000001;
                continue;
            }

            //Calculating the next z ( z_(n+1) = z_n - f(z_n)/f'(z_n) )
            zr -= (f_zr * f_zr_p + f_zi * f_zi_p) / f_z_p_length;
            zi -= (f_zr_p * f_zi - f_zi_p * f_zr) / f_z_p_length;
        }

        //Calculating which root is the closest
        int closest_root = -1;
        double root_distance = -1.0;
        for (int i = 0; i < n; i++) {
            double dr = zr - roots_r[i];
            double di = zi - roots_i[i];
            double d2 = di * di + dr * dr;
            if (root_distance == -1 || d2 < root_distance) {
                closest_root = i;
                root_distance = d2;
            }
        }

        //Saving the closest root and how many iterations it took to reach it before finding the real iteration range in c++
        image[(py * size + px) * 4] = (uint8)closest_root;
        image[(py * size + px) * 4 + 1] = (uint8)iterations;
    }
}


//Setting a color for every pixel
export void save_image(uniform int n, uniform int size, uniform uint8* uniform image, uniform uint8* uniform range) {
    uint8 least = range[0];
    uint8 most = range[1];

    //Defining the color theme
    struct ColorTheme { double r, g, b; };
    ColorTheme red   = {1.0, 0.333, 0.333};
    ColorTheme green = {0.314, 0.980, 0.482};
    ColorTheme blue  = {0.251, 0.376, 0.906};

    //Iterating over every pixel in parallel
    foreach (py = 0 ... size, px = 0 ... size) {
        //Calculating hue by the root and brightness by iterations
        double hue = (double)image[(py * size + px) * 4] / (double)n * 6.0;
        double tmp = ((double)image[(py * size + px) * 4 + 1] - least) / ((double)most - least);
        double brightness = 1.0 - tmp;

        //Converting HSV to RGB
        double X = brightness * (1.0 - abs(fmod(hue, 2.0) - 1.0));
        double R = 0.0, G = 0.0, B = 0.0;
        if (hue < 1.0) {
            R = red.r * brightness + green.r * X;
            G = red.g * brightness + green.g * X;
            B = red.b * brightness + green.b * X;
        }
        else if (hue < 2.0) {
            R = red.r * X + green.r * brightness;
            G = red.g * X + green.g * brightness;
            B = red.b * X + green.b * brightness;
        }
        else if (hue < 3.0) {
            R = green.r * brightness + blue.r * X;
            G = green.g * brightness + blue.g * X;
            B = green.b * brightness + blue.b * X;
        }
        else if (hue < 4.0) {
            R = green.r * X + blue.r * brightness;
            G = green.g * X + blue.g * brightness;
            B = green.b * X + blue.b * brightness;
        }
        else if (hue < 5.0) {
            R = blue.r * brightness + red.r * X;
            G = blue.g * brightness + red.g * X;
            B = blue.b * brightness + red.b * X;
        }
        else {
            R = blue.r * X + red.r * brightness;
            G = blue.g * X + red.g * brightness;
            B = blue.b * X + red.b * brightness;
        }

        //Saving the calculated color in an array ready for sf::Image
        int current_index = (py * size + px) * 4;
        image[current_index + 0] = (uint8)clamp((int)(255.0 * R), 0, 255);
        image[current_index + 1] = (uint8)clamp((int)(255.0 * G), 0, 255);
        image[current_index + 2] = (uint8)clamp((int)(255.0 * B), 0, 255);
        image[current_index + 3] = 255;
    }
}