#include "camera.hpp"

Camera::Camera(glm::vec3 pos, float FAV, float speed_move, float speed_rotate, float speed_jump, float near_dis, float far_dis) {
	Camera_pos = pos;
	Target_point = glm::vec3(0,0,-1);
	Side_vector = glm::cross(Camera_up, Target_point);
	Forward_vector = glm::cross(Camera_up, Side_vector);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	speed_m = speed_move;
	speed_r = speed_rotate;
	speed_j = speed_jump;
	FAV_game = FAV;
	near = near_dis;
	far = far_dis;
}


glm::mat4 Camera::getProjection_matrix(float Width_camera, float Hight_camera)
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(FAV_game), Width_camera / static_cast<float>(Hight_camera), near, far);
	return projection;
}


glm::mat4 Camera::getModelMatrix(glm::vec3 Position, float* Angle_rotate, glm::vec3* Vector_rotate)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);

	for (int i = 0; i < 3; i++) {
		model = glm::rotate(model, glm::radians(Angle_rotate[i]), Vector_rotate[i]);
	}

	return model;
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(Camera_pos, Camera_pos + Target_point, Camera_up);
	return view;
}

void Camera::moving(float* Move) 
{
	// if (Is_jumping){
	// 	float time_jump = (std::clock() - this->time_start_jump);
	// 	time_jump /= 1000;
	// 	float jump_hight;

	// 	if (Is_falling){
	// 		jump_hight = -G_CONSTANT*std::pow(time_jump, 2)/2;
	// 	}
	// 	else {jump_hight = (this->speed_j*time_jump - G_CONSTANT*std::pow(time_jump, 2)/2);}
	// 	if (jump_hight < 0 && !Is_falling){
	// 		this->time_start_jump = std::clock();
	// 		this->Is_falling = true;
	// 	}
		
	// 	Camera_pos = glm::vec3(Camera_pos.x, 
	// 						Camera_pos.y + jump_hight, 
	// 						Camera_pos.z);
	// }
	// if (Camera_pos.y - PLAYER_HIGHT < HIGHT_MAP){
	// 	Camera_pos.y = HIGHT_MAP + PLAYER_HIGHT;
	// 	this->Is_jumping = false;
	// 	this->Is_falling = false;
	// }

	if (!Is_moving){return;}
	float time_move = (std::clock() - this->time_norm) ;
	Camera_pos = glm::vec3(Camera_pos.x + Forward_vector.x * Move[2] * speed_m *time_move, 
						   Camera_pos.y, 
						   Camera_pos.z + Forward_vector.z * Move[2] * speed_m *time_move);
	
	Camera_pos = glm::vec3(Camera_pos.x + Side_vector.x * Move[0] * speed_m *time_move, 
						   Camera_pos.y, 
						   Camera_pos.z + Side_vector.z * Move[0] * speed_m *time_move);

	Camera_pos = glm::vec3(Camera_pos.x, 
						   Camera_pos.y + Move[1] * speed_m *time_move, 
						   Camera_pos.z);
	this->time_norm = std::clock();
}

void Camera::rotate(int Rotate[], bool fixed_target)
{
	// rotate the target _point
	float yaw = Angle_rotated[0] + Rotate[0] * speed_r;
	float pitch = Angle_rotated[1] + Rotate[1] * speed_r;
	if ((yaw <= -90 || yaw >= 90)) { yaw = Angle_rotated[0]; std::cout << "1"; }
	else {Angle_rotated[0] += Rotate[0] * speed_r;}
	
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(yaw));
	front.z = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
	//std::cout << front.x << std::endl;
	Target_point = glm::normalize(front);
	

	// rotate the forward amd side vector
	pitch = Angle_rotated[1] + Rotate[1] * speed_r + 180;
	
	front.x = cos(glm::radians(pitch));
	front.y = 0;
	front.z = sin(glm::radians(pitch));
	//std::cout << front.x << std::endl;
	Forward_vector = glm::normalize(front);

	pitch = Angle_rotated[1] + Rotate[1] * speed_r + 270;
	
	front.x = cos(glm::radians(pitch));
	front.y = 0;
	front.z = sin(glm::radians(pitch));
	//std::cout << front.x << std::endl;
	Side_vector = glm::normalize(front);

	for (int i = 1; i < 3; i++) { Angle_rotated[i] += Rotate[i] * speed_r; }

}

void Camera::control_mouse(Window& game_win, int win_width,int win_hight){
    //screen.swap_mouse(win_width / 2, win_hight / 2);
	// std::cout<< "hell0";
	SDL_GetMouseState(&mouse_current_x, &mouse_current_y);
	/*std::cout << mouse_current_x << std::endl;
	std::cout << mouse_current_y << std::endl;*/
	if ((mouse_current_x - win_width/2 != 0 || mouse_current_y - win_hight/2 != 0) && SDL_GetRelativeMouseMode()) {
		int Rotate[] = { - mouse_current_y + win_hight/2, mouse_current_x - win_width/2, 0 };
		rotate(Rotate, false);
		game_win.swap_mouse(win_width / 2, win_hight / 2);
	}

}

void Camera::control_moving(){
	float move[3] = { 0,0,0 };
	const Uint8 *key = SDL_GetKeyboardState(NULL);
	int cout_key = 0;

	if (key[SDL_SCANCODE_W]){
		cout_key ++;
		move[2] -= 0.1;
	}
	if (key[SDL_SCANCODE_S]){
		cout_key ++;
		move[2] += 0.1;
	}
	if (key[SDL_SCANCODE_A]){
		cout_key ++;
		move[0] += 0.1;
	}
	if (key[SDL_SCANCODE_D]){
		cout_key ++;
		move[0] -= 0.1;
	}
	if (key[SDL_SCANCODE_SPACE]){
		move[1] += 0.1;
	}
	if (key[SDL_SCANCODE_LSHIFT]){
		move[1] -= 0.1;
	}
	if (Is_moving){if (move[0] == 0 && move[1] == 0 && move[2] == 0) Is_moving = false;}
	else{
	if (move[0] != 0 || move[1] != 0 || move[2] != 0){
		Is_moving = true;
		this->time_norm = std::clock();	
	}}

	if (cout_key == 2){
		move[0] = move[0] / std::sqrt(2);
		move[2] = move[2] / std::sqrt(2);
	}

	moving(move);
}