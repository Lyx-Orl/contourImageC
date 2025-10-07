//import lib
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//pixel
typedef struct {
    unsigned char r, g, b, a;
} Color;

//euclidan distance, pythagor
float distance_between_two_point(int x1, int y1, int x2, int y2){
    return sqrtf((x2-x1)*(x2-x1)+((y2-y1)*(y2-y1)));
}

int is_opaque(unsigned char alpha){
    return alpha >= 50;
}

int main(int argc, char* argv[]){
    
    //Check arguments
    if (argc != 8){
        printf("Not enough or too many arguments");
        printf("Format: %s <input.png> <size_of_contour> <R> <G> <B> <A> <output.png>\n", argv[0]);
        return 1;
    }

    //contour color
    Color contour_color = {
        (unsigned char)atoi(argv[3]),
        (unsigned char)atoi(argv[4]),
        (unsigned char)atoi(argv[5]),
        (unsigned char)atoi(argv[6])
        
    };

    // variable for load image
    const char* input_path = argv[1];
    int distance = atoi(argv[2]);
    const char* output_path = argv[7];

    //load image
    int width, height, channels;
    unsigned char* original_data = stbi_load(input_path, &width, &height, &channels, 4);
    if (!original_data) {
        printf("Error : impossible to load image %s\n", input_path);
        return 1;
    }

    /*
    format of image load
    r,g,b,a are unsigned char (that the 4 in stdi_load which give alpha, if we put 3 we don;t have alpha)
    image :
    [r,g,b,a],[r,g,b,a],[r,g,b,a],[r,g,b,a],[r,g,b,a] .... (height*width which is the number of pixel of the image)
    */

    //variables for new image
    int new_width = width + 2 * distance;
    int new_height = height + 2 * distance;
    unsigned char* new_image = (unsigned char*)malloc((new_width * new_height * 4)*sizeof(unsigned char));
    if (!new_image) {
        printf("Memory error\n");
        stbi_image_free(original_data);
        return 1;
    }

    // Copy of the image in the new image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int orig_index = (y * width + x) * 4;
            int new_index = ((y + distance) * new_width + (x + distance)) * 4; //new position of the pixel in the image
            // we make a bigger image to avoid to put the contour beyond the borders

            new_image[new_index]     = original_data[orig_index];     // R
            new_image[new_index + 1] = original_data[orig_index + 1]; // G
            new_image[new_index + 2] = original_data[orig_index + 2]; // B
            new_image[new_index + 3] = original_data[orig_index + 3]; // A
            
        }
    }

    // Add contour
    for (int y = 0; y < new_height; y++){
        for (int x = 0; x < new_width; x++) {

            int current_index = (y * new_width + x) * 4;
            // not transparent -> continue
            if (is_opaque(new_image[current_index + 3])) {
                continue;
            }

            int coloration = 0;

            //check on square of size 2 dist around our pixel
            for (int dy = -distance; dy <= distance; dy++){
                for (int dx = -distance; dx <= distance; dx++){
                    
                    int nx = x + dx;
                    int ny = y + dy;

                    // pixel in the image
                    if (nx >= 0 && nx < new_width && ny >= 0 && ny < new_height) {
                        int index_pixel = (ny*new_width + nx) * 4;

                        if (is_opaque(new_image[index_pixel + 3])) {
                            float dist = distance_between_two_point(x, y, nx, ny);
                            if (dist < distance) {
                                coloration = 1;
                            }
                        }
                    }
                }
            }

            if (coloration == 0){
                new_image[current_index]     = contour_color.r;
                new_image[current_index + 1] = contour_color.g;
                new_image[current_index + 2] = contour_color.b;
                new_image[current_index + 3] = contour_color.a;
            }else{
                new_image[current_index]     = 0;
                new_image[current_index + 1] = 0;
                new_image[current_index + 2] = 0;
                new_image[current_index + 3] = 0;
            }
        }
    }

    if (!stbi_write_png(output_path, new_width, new_height, 4, new_image, new_width * 4)) {
        printf("Erreur: Impossible de sauvegarder l'image %s\n", output_path);
    } else {
        printf("Image sauvegardée avec succès: %s\n", output_path);
    }

    stbi_image_free(original_data);
    free(new_image);

    return 0;

}