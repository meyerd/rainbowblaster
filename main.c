#include <psputils.h>
#include <time.h>
#include <stdlib.h>
#include <oslib/oslib.h>

PSP_MODULE_INFO("Rainbow Blaster", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define NUM_BACKGROUND_STARS 20
#define NUM_ENEMIES 15
#define NUM_ENEMY_TYPES 2

OSL_IMAGE* ray;
OSL_IMAGE* character;
OSL_IMAGE* background_star;
OSL_IMAGE* enemy_types[NUM_ENEMY_TYPES];
int cposx;
int cposy;
int tmp_ray_anim_phase;
int ray_anim_phase;
int ray_on;

int background_star_positions[NUM_BACKGROUND_STARS][2] = {{0}};
int tmp_background_cnt;

struct enemy_t {
	int x;
	int ycenter;
	int y;
	float amplitude;
	int width;
	int height;
	int state;
	int type;
};

typedef struct enemy_t enemy;

enemy enemies[NUM_ENEMIES];
int tmp_enemy_cnt;

int score;
char scorestr[100] = {0};

void CheckKeys();
void AnimateRay();
void AnimateBackground();
void DrawCharacter();
void DrawBackground();
void CheckCollision();
void AnimateEnemies();
void DrawEnemies();

int inline GetRandomInt(int min, int max) {
	return min+rand()%(max-min);
}

int main()
{

	oslInit(0);
 
	oslInitGfx(OSL_PF_8888, 1);

	srand(time(NULL));
 
	oslSetTransparentColor(RGB(255,0,255));
 
	ray = oslLoadImageFile("ray_sprite.png", OSL_IN_VRAM, OSL_PF_5650);
	character = oslLoadImageFile("character_main.png", OSL_IN_VRAM, OSL_PF_5551);
	background_star = oslLoadImageFile("background_star.png", OSL_IN_VRAM, OSL_PF_5551);
	enemy_types[0] = oslLoadImageFile("enemy_01.png", OSL_IN_VRAM, OSL_PF_5551);

	oslDisableTransparentColor();
 
	if (!ray || !character || !background_star)
		oslDebug("Check if all the files are copied in the game folder.");

	
	int i = 0;
	for(i = 0; i < NUM_BACKGROUND_STARS; i++) {
		background_star_positions[i][0] = GetRandomInt(0,480-20);
		background_star_positions[i][1] = GetRandomInt(0,240-20);
	}
	tmp_background_cnt = 0;

	for(i = 0; i < NUM_ENEMIES; i++) {
		enemies[i].x = -1;
		enemies[i].y = -1;
		enemies[i].ycenter = -1;
		enemies[i].amplitude = 0.0f;
		enemies[i].width = 25;
		enemies[i].height = 25;
		enemies[i].state = 1;
		enemies[i].type = 0;
	}

	ray->x = 0;
	ray->y = 0;
	character->x = 0;
	character->y = 0;
	cposx = 0;
	cposy = 272/2 - 37;
	ray_on = 0;
	ray_anim_phase = 0;
	tmp_ray_anim_phase = 0;
	score = 0;
	int skip = 0;

	while (!osl_quit)
	{
		CheckKeys();
		CheckCollision();
		AnimateBackground();
		AnimateEnemies();
		if(!skip) {
			oslStartDrawing();		
			DrawBackground();
			DrawEnemies();
			DrawCharacter();
			sprintf(scorestr, "Score: %i", score);
			oslDrawString(380, 10, scorestr);
			oslEndDrawing();
		}
 
		skip = oslSyncFrame();		
	}
	
	oslEndGfx();
	oslQuit();
	return 0;
}
 
 void CheckKeys() {
	oslReadKeys();
	if(osl_keys->held.left) {
		cposx -= 3;
	}
	if(osl_keys->held.right) {
		cposx += 3;
	}
	if(osl_keys->held.up) {
		cposy -= 3;
	}
	if(osl_keys->held.down) {
		cposy += 3;
	}

	if(cposx < -85) {
		cposx = -85;
	}
	if(cposx > 343) {
		cposx = 343;
	}
	if(cposy < -19) {
		cposy = -19;
	}
	if(cposy > 259) {
		cposy = 259;
	}

	if(osl_keys->held.cross) {
		AnimateRay();
		ray_on = 1;
	} else {
		ray_anim_phase = 0;
		tmp_ray_anim_phase = 0;
		oslSetImageTileSize(ray, ray_anim_phase * 6, ray_anim_phase * 6 + 6, 80, 6);
		ray_on = 0;
	}
}

void DrawCharacter() {
	character->x = cposx;
	character->y = cposy;
	oslDrawImage(character);
	if(ray_on) {
		int i;
		for(i = 0; i < 6; i++) {
			ray->x = cposx + 137 + 80 * i;
			ray->y = cposy + 19;
			oslDrawImage(ray);
		}
	}
}

void AnimateRay() {
	tmp_ray_anim_phase++;
	if(tmp_ray_anim_phase == 3) {
		tmp_ray_anim_phase = 0;
		ray_anim_phase++;
		oslSetImageTileSize(ray, 0, ray_anim_phase * 6, 80, 6);
		if(ray_anim_phase == 5) {
			ray_anim_phase = 0;
		}
	}
}

void AnimateBackground() {
	tmp_background_cnt++;
	if(tmp_background_cnt == 5) {
		tmp_background_cnt = 0;
		int i;
		for(i = 0; i < NUM_BACKGROUND_STARS; i++) {
			background_star_positions[i][0] -= 2;
			if(background_star_positions[i][0] < -20) {
				background_star_positions[i][0] = 480 + GetRandomInt(0, 400);
				background_star_positions[i][1] = GetRandomInt(0,240-20);
			}
		}
	}
}

void DrawBackground() {
	oslDrawGradientRect(0,0,480,250, RGB(177,10,175), RGB(177,10,175), RGB(225,200,225), RGB(225,200,225));
	oslDrawGradientRect(0,250,480,272, RGB(46,109,5), RGB(46,109,5), RGB(95,210,19), RGB(95,210,19));
	int i;
	for(i = 0; i < NUM_BACKGROUND_STARS; i++) {
		if(background_star_positions[i][0] < 480) {
			background_star->x = background_star_positions[i][0];
			background_star->y = background_star_positions[i][1];
			oslDrawImage(background_star);
		}
	}
}

void AnimateEnemies() {
	tmp_enemy_cnt++;
	if(tmp_enemy_cnt == 2) {
		tmp_enemy_cnt = 0;
		int i;
		for(i = 0; i < NUM_ENEMIES; i++) {
			if(enemies[i].y <= cposy + 19 && cposy + 19 <= enemies[i].y + 25 && 
				enemies[i].state == 0 && enemies[i].x < (480-2) && ray_on && cposx + 137 < enemies[i].x) {
				enemies[i].state = 1;
			}
			enemies[i].x -= 2;
			enemies[i].y = enemies[i].ycenter + (int)oslSin((float)enemies[i].x * 2.0f, enemies[i].amplitude);
			if(enemies[i].x < -25) {
				if(enemies[i].state == 0)
					score -= 1;
				enemies[i].x = 480 + GetRandomInt(0, 400);
				enemies[i].ycenter = GetRandomInt(0, 260-25);
				enemies[i].amplitude = (float)GetRandomInt(3, 50);
				enemies[i].state = 0;
			}
		}
	}
}

void DrawEnemies() {
	int i;
	for(i = 0; i < NUM_ENEMIES; i++) {
		if(enemies[i].state == 0 && enemies[i].x < 480) {
			enemy_types[enemies[i].type]->x = enemies[i].x;
			enemy_types[enemies[i].type]->y = enemies[i].y;
			oslDrawImage(enemy_types[enemies[i].type]);
		}
	}
}

void CheckCollision() {

}