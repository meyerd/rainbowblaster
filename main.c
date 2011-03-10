//The main library OSLib
#include <oslib/oslib.h>
 
PSP_MODULE_INFO("Rainbow Blaster", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
 
OSL_IMAGE* ray;
OSL_IMAGE* character;
int cposx;
int cposy;
int tmp_ray_anim_phase;
int ray_anim_phase;
int ray_on;

void CheckKeys();
void AnimateRay();
void DrawCharacter();

int main()
{

	oslInit(0);
 
	oslInitGfx(OSL_PF_8888, 1);
 
	oslSetTransparentColor(RGB(255,0,255));
 
	ray = oslLoadImageFile("ray_sprite.png", OSL_IN_VRAM, OSL_PF_5650);
	character = oslLoadImageFile("character_main.png", OSL_IN_VRAM, OSL_PF_5551);

	oslDisableTransparentColor();
 
	if (!ray || !character)
		oslDebug("Check if all the files are copied in the game folder.");

	ray->x = 0;
	ray->y = 0;
	character->x = 0;
	character->y = 0;
	cposx = 0;
	cposy = 272/2 - 37;
	ray_on = 0;
	ray_anim_phase = 0;
	tmp_ray_anim_phase = 0;
	int skip = 0;

	while (!osl_quit)
	{
		CheckKeys();
		if(!skip) {
			oslStartDrawing();		
			oslDrawFillRect(0,0,480,272,RGB(0,0,20));
			DrawCharacter();
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
