#include <raylib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PADDLE_WIDTH 20.0f
#define PADDLE_HEIGHT 150.0f 
#define PADDLE_MARGIN 30.0f
#define BALL_RADIUS 15.0f

typedef struct {
  float x, y;
  float speed;
  int score;
} Paddle;

typedef struct {
  float x, y;
  float speedX, speedY;
  float radius;
} Ball;

// Function to put back the ball in the center of the screen
void ResetGame(Ball *ball, Paddle *player, Paddle *ai) {
  ball->x = SCREEN_WIDTH / 2.0f;
  ball->y = SCREEN_HEIGHT / 2.0f;

  // Random direction for the service
  ball->speedX = (GetRandomValue(0, 1) == 0) ? 550.0f : -550.0f;
  ball->speedY = (GetRandomValue(0, 1) == 0) ? 550.0f : -550.0f;

  player->y = (SCREEN_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
  ai->y = (SCREEN_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PONG");
  SetTargetFPS(144);

  Paddle player = {PADDLE_MARGIN, 0, 600.0f, 0};
  Paddle ai = {SCREEN_WIDTH - PADDLE_MARGIN - PADDLE_WIDTH, 0, 550.0f, 0};
  Ball ball = {0, 0, 0, 0, BALL_RADIUS};

  ResetGame(&ball, &player, &ai);
  bool gameOver = false;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (!gameOver) {
      // --- Logic Update ---
      ball.x += ball.speedX * dt;
      ball.y += ball.speedY * dt;

      // Wall collisions (Top/Bottom)
      if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREEN_HEIGHT) {
        ball.speedY *= -1.0f;
      }

      // Paddle collisions
      Rectangle playerRec = {player.x, player.y, PADDLE_WIDTH, PADDLE_HEIGHT};
      Rectangle aiRec = {ai.x, ai.y, PADDLE_WIDTH, PADDLE_HEIGHT};

      if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius,
                                  playerRec)) {
        ball.speedX = (ball.speedX < 0) ? -ball.speedX * 1.05f : ball.speedX;
        ball.x = player.x + PADDLE_WIDTH + ball.radius;
      }
      if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius,
                                  aiRec)) {
        ball.speedX = (ball.speedX > 0) ? -ball.speedX * 1.05f : ball.speedX;
        ball.x = ai.x - ball.radius;
      }

      // Scoring logic
      if (ball.x < 0) {
        ai.score++;
        gameOver = true;
      } else if (ball.x > SCREEN_WIDTH) {
        player.score++;
        gameOver = true;
      }

      // Inputs
      if (IsKeyDown(KEY_W))
        player.y -= player.speed * dt;
      if (IsKeyDown(KEY_S))
        player.y += player.speed * dt;

      // AI tracking
      if (ball.y < ai.y + PADDLE_HEIGHT / 2.0f)
        ai.y -= ai.speed * dt;
      if (ball.y > ai.y + PADDLE_HEIGHT / 2.0f)
        ai.y += ai.speed * dt;

      // Clamp paddles
      if (player.y < 0)
        player.y = 0;
      if (player.y + PADDLE_HEIGHT > SCREEN_HEIGHT)
        player.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
      if (ai.y < 0)
        ai.y = 0;
      if (ai.y + PADDLE_HEIGHT > SCREEN_HEIGHT)
        ai.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

    } else {
      // --- Restart logic ---
      if (IsKeyPressed(KEY_SPACE)) {
        ResetGame(&ball, &player, &ai);
        gameOver = false;
      }
    }

    // --- Drawing ---
    BeginDrawing();
    ClearBackground((Color){20, 20, 25, 255}); 
    // Draw Middle Line
    DrawLineEx((Vector2){SCREEN_WIDTH / 2.0, 0},
               (Vector2){SCREEN_WIDTH / 2.0, SCREEN_HEIGHT}, 2, DARKGRAY);

    // Draw HUD (Scores)
    DrawText(TextFormat("%d", player.score), SCREEN_WIDTH / 4, 50, 60, WHITE);
    DrawText(TextFormat("%d", ai.score), 3 * SCREEN_WIDTH / 4, 50, 60, WHITE);

    if (gameOver) {
      DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                    (Color){0, 0, 0, 150}); // Overlay
      const char *msg = "PRESS [SPACE] TO CONTINUE";
      DrawText(msg, SCREEN_WIDTH / 2 - MeasureText(msg, 30) / 2,
               SCREEN_HEIGHT / 2, 30, YELLOW);
    } else {
      DrawCircleV((Vector2){ball.x, ball.y}, ball.radius, WHITE);
      DrawRectangleRounded(
          (Rectangle){player.x, player.y, PADDLE_WIDTH, PADDLE_HEIGHT}, 0.5, 5,
          WHITE);
      DrawRectangleRounded((Rectangle){ai.x, ai.y, PADDLE_WIDTH, PADDLE_HEIGHT},
                           0.5, 5, WHITE);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
