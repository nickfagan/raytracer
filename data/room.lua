-- A simple scene with five spheres

boxMat = gr.material({0.3, 0.3, 0.3}, {0.3, 0.3, 0.3}, 5)
mat1 = gr.ref_material({0.5, 0.5, 0.5}, {0.7, 0.7, 0.7}, 10, 0.5)
mat2 = gr.ref_material({0.8, 0.0, 0.0}, {1.0, 1.0, 1.0}, 25, 0.5)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {-60, -60, -60}, 40)
scene_root:add_child(s1)
s1:set_ref_material(mat2)

---------- WALLS --------------
--back
back_wall = gr.mesh('back_wall', {
	{-100.0, -100.0, -100.0},
	{100.0, -100.0, -100.0},
	{100.0, 200.0, -100.0},
	{-100.0, 200.0, -100.0}
     }, {
	{0, 1, 2, 3}
     })

back_wall:set_material(boxMat)
scene_root:add_child(back_wall)

--bottom
bottom_wall = gr.mesh('bottom_wall', {
	{-100.0, -100.0, -100.0},
	{-100.0, -100.0, 501.0},
	{100.0, -100.0, 501.0},
	{100.0, -100.0, -100.0}
     }, {
	{0, 1, 2, 3}
     })

bottom_wall:set_material(boxMat)
scene_root:add_child(bottom_wall)

--left
left_wall = gr.mesh('left_wall', {
	{-100.0, -100.0, -100.0},
	{-100.0, 200.0, -100.0},
	{-100.0, 200.0, 501.0},
	{-100.0, -100.0, 501.0}
     }, {
	{0, 1, 2, 3}
     })

left_wall:set_material(boxMat)
scene_root:add_child(left_wall)

--right
right_wall = gr.mesh('right_wall', {
	{100.0, 200.0, 501.0},
	{100.0, 200.0, -100.0},
	{100.0, -100.0, -100.0},
	{100.0, -100.0, 501.0}
     }, {
	{0, 1, 2, 3}
     })

right_wall:set_material(boxMat)
scene_root:add_child(right_wall)

--top
top_wall = gr.mesh('top_wall', {
	{100.0, 200.0, 501.0},
	{-100.0, 200.0, 501.0},
	{-100.0, 200.0, -100.0},
	{100.0, 200.0, -100.0}
     }, {
	{0, 1, 2, 3}
     })

top_wall:set_material(boxMat)
scene_root:add_child(top_wall)

--front
front_wall = gr.mesh('back_wall', {
	{100.0, 200.0, 501.0},
	{-100.0, 200.0, 501.0},
	{-100.0, -100.0, 501.0},
	{100.0, -100.0, 501.0}
     }, {
	{0, 1, 2, 3}
     })

front_wall:set_material(boxMat)
scene_root:add_child(front_wall)

white_light = gr.box_light({80.0, 70.0, 0.0}, {1.0, 1.0, 1.0}, {1, 0.00001, 0.00001}, 1.0, 5)
blue_light = gr.box_light({-80.0, 70.0, 0.0}, {0.0, 0.8, 1.0}, {1, 0.00001, 0.00001}, 1.0, 5)

gr.render(scene_root, 'room.png', 768, 512,
	  {0, -21, 500}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, blue_light})




