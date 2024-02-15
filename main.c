#include <stdio.h>
#include "raylib.h"

const int screen_width = 1280;
const int screen_height = 800;
int player_score = 0;
int ai_score = 0;

typedef struct {
    int x;
    int y;
    int speed_x, speed_y;
    int radius;
} Ball;

typedef struct {
    int x, y;
    int width, height;
    int speed;
} Paddle;

typedef struct 
{
    int x, y;
    int width, height;
    int speed;
} AI_Paddle;


// Entities
Ball ball;
Paddle player1;
AI_Paddle ai_paddle;


void Draw_Ball()
{
    DrawCircle(ball.x, ball.y, ball.radius, WHITE);
}

void Update_Ball()
{
    ball.x += ball.speed_x;
    ball.y += ball.speed_y;

    if (ball.y + ball.radius >= screen_height || ball.y - ball.radius <= 0)
        ball.speed_y *= -1;

    if (ball.x + ball.radius >= screen_width || ball.x - ball.radius <= 0)
        ball.speed_x *= -1;
    
    if (ball.x + ball.radius >= screen_width)
        ai_score++;
    
    if (ball.x - ball.radius <= 0)
        player_score++;

}

void Draw_Paddle(int* x, int* y, int* width, int* height) 
{
    DrawRectangle(*x, *y, *width, *height, WHITE);
}

void Update_Paddle(Paddle* paddle)
{
    if (IsKeyDown(KEY_UP))
        paddle->y = paddle->y - paddle->speed;

    if (IsKeyDown(KEY_DOWN))
        paddle->y = paddle->y + paddle->speed;

    if (paddle->y <= 0)
        paddle->y = 0;
    
    if (paddle->y + paddle->height >= screen_height)
        paddle->y = screen_height - paddle->height;
}
void Update_AI_Paddle(AI_Paddle* ai_paddle)
{
    if (ai_paddle->y + ai_paddle->height > ball.y)
        ai_paddle->y = ai_paddle->y - ai_paddle->speed;
    if (ai_paddle->y + ai_paddle->height / 2 <= ball.y)
        ai_paddle->y = ai_paddle->y + ai_paddle->speed;

    if (ai_paddle->y <= 0)
        ai_paddle->y = 0;
    
    if (ai_paddle->y + ai_paddle->height >= screen_height)
        ai_paddle->y = screen_height - ai_paddle->height;
}

int main()
{
    printf("Starting the game\n");
    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);

    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    ball.radius = 13;

    player1.width = 25;
    player1.height = 120;
    player1.x = (screen_width - player1.width - 10);
    player1.y = screen_height / 2 - player1.height / 2;
    player1.speed = 6;

    ai_paddle.width = 25;
    ai_paddle.height = 120;
    ai_paddle.x = 10;
    ai_paddle.y = screen_height / 2 - player1.height / 2;
    ai_paddle.speed = 6;

    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        Draw_Ball(&ball.x, &ball.y, ball.radius);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        Draw_Paddle(&player1.x, &player1.y, &player1.width, &player1.height);
        Draw_Paddle(&ai_paddle.x, &ai_paddle.y, &ai_paddle.width, &ai_paddle.height);
        DrawText(TextFormat("%i", ai_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);


        // Check collision
        Vector2 ball_vec = {.x = ball.x, .y = ball.y};
        Rectangle player1_rect = {.x = player1.x, .y = player1.y, .width = player1.width, .height = player1.height};
        Rectangle ai_paddle_rect = {.x = ai_paddle.x, .y = ai_paddle.y, .width = ai_paddle.width, .height = ai_paddle.height};
        if (CheckCollisionCircleRec(ball_vec, ball.radius, player1_rect))
            ball.speed_x *= -1;

        if (CheckCollisionCircleRec(ball_vec, ball.radius, ai_paddle_rect))
            ball.speed_x *= -1;

        Update_Ball();
        Update_Paddle(&player1);
        Update_AI_Paddle(&ai_paddle);

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
