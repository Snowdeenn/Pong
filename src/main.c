#include <raylib.h>
#include <stdlib.h>

#define BORDURE_HAUT 0
#define BORDURE_BAS 1040
#define BORDURE_GAUCHE 0
#define BORDURE_DROIE 1880

typedef enum {

    HAUT = KEY_W,
    BAS = KEY_S,

} ToucheJoueur;

int main(void) {

//  Initialistation de la fenêtre
    InitWindow(1880, 1040, "PONG");
    SetTargetFPS(144);

    float yJoueur = (BORDURE_BAS / 2) - 120;    // Axe de mouvement de la raquette du joueur
    float vRecJoueur = 500;                     // Vitesse de la raquette du joueur

    float yIA = (BORDURE_BAS / 2) - 120;        // Axe de mouvement de la raquette de l'ordinateur
    float vRecIA = 700;                         // Vitesse de la raquette de l'ordinateur

    float xBalle = 940, yBalle = 520;   // Coordonée de base de la balle 
    float vXBalle = 550, vYBalle = 600; // Vitesse de la balle en pixels/secondes
    float rayonBalle = 20;              // Rayon de la balle

    int scoresJoueur = 0;
    int scoresIA = 0;

    int gamerOver = 0;


//  Boucle de jeu
    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

//      Mise à jour de la position de la balle
        xBalle += vXBalle * dt;
        yBalle += vYBalle * dt;

        Rectangle raquetteIA = {

            (float)(BORDURE_DROIE - 30 - 20),
            yIA,
            20.0f,
            240.0f
        };
    
        Rectangle raquetteJoueur = {

            (float)(BORDURE_GAUCHE + 30),
            yJoueur,
            20.0f,  // Largeur du rectangle
            240.0f  // Longueur du rectangle

        };

//      Si la balle arrive sur une bordure on inverse sa vitesse et augmente sur l'axe des ordonnées de 5%
        if(yBalle - rayonBalle <= BORDURE_HAUT || yBalle + rayonBalle >= BORDURE_BAS) {
            vYBalle *= -1.05f;    
        }

//      Si la balle touche la raquetteIA on inverse et augmente sa vitesse sur l'axe des abcisses de 5%
        if(CheckCollisionCircleRec((Vector2){xBalle, yBalle}, rayonBalle, raquetteIA)) {
            vXBalle *= -1.05f;
            scoresIA++;   
        }

//      Si la balle touche la raquetteJoueur on inverse et augmente sa vitesse sur l'axe des abcisses de 5%
        if(CheckCollisionCircleRec((Vector2){xBalle, yBalle}, rayonBalle, raquetteJoueur)) {
            vXBalle *= -1.05f;
            scoresJoueur++;
        }
        
//      Si la balle arrive aux bordures droite ou gauche on passe gameOver à 1
        if(xBalle + rayonBalle <= BORDURE_GAUCHE || xBalle - rayonBalle >= BORDURE_DROIE) {
            gamerOver = 1;
           }

//      Si la raquette du joueur attend le bas de la fenêtre
        if(yJoueur + 240 >= BORDURE_BAS) {
            yJoueur = BORDURE_BAS - 240;
        }

//      Si la raquette du joueur attend le haut de la fenêtre
        if(yJoueur <= BORDURE_HAUT) {
            yJoueur = BORDURE_HAUT;
        }

//      Si la raquette de l'IA attend le bas de la fenêtre
        if(yIA + 240 >= BORDURE_BAS) {
            yIA = BORDURE_BAS - 240;
        }

//      Si la raquette de l'IA attend le haut de la fenêtre
        if(yIA <= BORDURE_HAUT) {
            yIA = BORDURE_HAUT;
        }
        
/////////////////// Gestion des déplacement ///////////////////

//      Si z est appuyée on monte              
        if(IsKeyDown(HAUT)) yJoueur += vRecJoueur * dt * -1;
//      Si s est appuyée on desecend
        if(IsKeyDown(BAS)) yJoueur += vRecJoueur * dt;

//      L'ia suit la balle
        if(yBalle - 120 < yIA) {
            yIA += vRecIA * dt * -1;
        }

        if(yBalle - 120 > yIA) {
            yIA += vRecIA * dt;
        }

        BeginDrawing();

            ClearBackground(BLACK);

//          Si gameOver = 1 alors la partie est fini sinon on continue
            if(gamerOver == 1) {
                
                const char *txtJoueur = TextFormat("Score joueur : %d", scoresJoueur);
                const char *txtIA = TextFormat("Score IA : %d", scoresIA);
                const char *txtGO = "GAME OVER";
                
                DrawText(txtGO, GetScreenWidth()/2 - MeasureText(txtGO, 40)/2, 480, 40, WHITE);
                DrawText(txtJoueur, GetScreenWidth()/3 - MeasureText(txtJoueur, 30)/2, 560, 30, WHITE);
                DrawText(txtIA, 2*GetScreenWidth()/3 - MeasureText(txtIA, 30)/2, 560, 30, WHITE);
            
            } else {
                
                DrawCircle((int)xBalle, (int)yBalle, rayonBalle, WHITE);
                DrawRectangle(BORDURE_GAUCHE + 30, (int)yJoueur, 20, 240, WHITE);
                DrawRectangle(BORDURE_DROIE - 30 - 20, (int)yIA, 20, 240, WHITE);

            }

        EndDrawing();

    }

    CloseWindow();
    return EXIT_SUCCESS;
}
