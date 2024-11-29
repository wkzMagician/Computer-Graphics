#include "include/mymodelwrap.h"
#include "include/modelwrap.h"
#include "include/terrain.h"
#include "include/physics_system.h"
#include "include/particle_emitter.h"

#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


bool firstMouse = true;

// 时间设置
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // 初始化并配置glfw
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 告诉 GLFW 捕获我们的鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad：加载所有 OpenGL 函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 配置全局 OpenGL 状态
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // 生成地形
	Terrain terrain(terrainRadius, terrainBaseHeight + terrainMinHeight, terrainBaseHeight + terrainMaxHeight, terrainRoughness, terrainTextureSize);
	terrain.setVerticesArray();
	float* terrainVertices = terrain.getVerticesArray();
	unsigned int terrainVerticesSize = terrain.getVerticesArraySize();

    // 加载数据
	initData();

    // 编译shader操作
    Shader lightingShader("resources/shader/lighting.vs", "resources/shader/lighting.fs");
    Shader lightCubeShader("resources/shader/lightcube.vs", "resources/shader/lightcube.fs");
    Shader modelShader("resources/shader/model.vs", "resources/shader/model.fs");
	Shader terrainShader("resources/shader/terrain.vs", "resources/shader/terrain.fs");
	Shader snowShader("resources/shader/snow.vs", "resources/shader/snow.fs");
	Shader starShader("resources/shader/star.vs", "resources/shader/star.fs");
	Shader treeShader("resources/shader/tree.vs", "resources/shader/tree.fs");
	Shader starlightShader("resources/shader/starlight.vs", "resources/shader/starlight.fs");

    // load models
    Model treeModel("resources/obj/christmas_tree/christmas_tree.obj");
    Model tableModel("resources/obj/table/table.obj");

    ///
	Model snowModel("resources/obj/snow/snow.obj");
    // 彩灯
	//Model starModel("resources/obj/star/star.obj");
    ///

	MyMesh FWall(FWallVertices, FWallVerticesSize, FWallColor);
	MyMesh LWall(LWallVertices, LWallVerticesSize, LWallColor);
	MyMesh RWall(RWallVertices, RWallVerticesSize, RWallColor);
	MyMesh Ceiling(CeilingVertices, CeilingVerticesSize, CeilingColor);
	MyMesh Floor(FloorVertices, FloorVerticesSize, FloorColor);
	MyMesh blackboard(blackboardVertices, blackboardVerticesSize, blackboardColor);
	MyMesh blackboardBorder(blackboardBorderVertices, blackboardBorderVerticesSize, blackboardBorderColor);
	MyMesh windmill(windmillVertices, windmillVerticesSize, windmillColor);
	windmill.setDrawEdge(true);
	MyMesh lightCube(vertices, lightCubeVerticesSize, lightCubeColor);

    MyMesh terrainMesh(terrainVertices, terrainVerticesSize, glm::vec3(1.0f, 1.0f, 1.0f), texturePaths, true);
    terrainMesh.setDrawTerrain(true);
	MyMesh terrainBase(terrainBaseVertices, terrainBaseVerticesSize, terrainBaseColor);
    MyMesh terrainTop(terrainTopVertices, terrainTopVerticesSize, terrainBaseColor);

	//MyMesh snowMesh(snowVertices, snowVerticesSize, glm::vec3(1.0f, 1.0f, 1.0f), snowTexturePaths, true);

	std::vector<MyMesh*> room = { &FWall, &LWall, &RWall, &Ceiling, &Floor};
	std::vector<MyMesh*> blackboardMeshes = { &blackboard, &blackboardBorder };
	std::vector<MyMesh*> windmillMeshes = { &windmill };
	std::vector<MyMesh*> lightCubeMeshes = { &lightCube };
	std::vector<MyMesh*> terrainMeshes = { &terrainMesh, &terrainBase, &terrainTop };
	//std::vector<MyMesh*> snowMeshes = { &snowMesh };

	MyModel roomModel(room);
	MyModel blackboardModel(blackboardMeshes);
	MyModel windmillModel(windmillMeshes);
	MyModel lightCubeModel(lightCubeMeshes);
	MyModel terrainModel(terrainMeshes);
	//MyModel snowModel(snowMeshes);

	ModelWrap treeModelWrap(&treeModel, &treeShader, treeGeometry);
	ModelWrap tableModelWrap(&tableModel, &modelShader, tableGeometry);
	
	MyModelWrap roomModelWrap(&roomModel, &lightingShader, roomGeometry);
	MyModelWrap blackboardModelWrap(&blackboardModel, &lightingShader, blackboardGeometry);
	MyModelWrap windmillModelWrap(&windmillModel, &lightingShader, windmillGeometry);
	MyModelWrap lightCubeModelWrap(&lightCubeModel, &lightCubeShader, lightCubeGeometry);
	MyModelWrap terrainModelWrap(&terrainModel, &terrainShader, terrainBaseGeometry);

	//MyModelWrap snowModelWrap(&snowModel, &lightingShader, Geometry(glm::vec3(0.0f), glm::vec3(0.05f), glm::vec3(0.0f)));
	ModelWrap snowModelWrap(&snowModel, &lightCubeShader, snowGeometry);
	/*ModelWrap starModelWrap(&starModel, &starShader, starGeometry);*/

	MyModelWrap lightModelWrap(&lightCubeModel, &starlightShader, lightGeometry);
	//lightModelWrap.setColor(glm::vec3(1.0f, 215.0f / 255.0f, 0.0f));
    lightModelWrap.setColor(glm::vec3(1.0f,0.0f, 0.0f));

	// 设置物理系统

	PhysicsSystem physicsSystem;
    Particle snow(&snowModelWrap);
	snow.addCollisionLayer(2);
    snow.addCollisionLayerMask(1);
	snow.setCollisionBox(new Point(glm::vec3(0.0f)));
	
	ParticleEmitter snowEmitter(&physicsSystem, snow, terrainPos + glm::vec3(0.0f, 0.15f, 0.0f));
	//ParticleEmitter snowEmitter(&physicsSystem, snow, glm::vec3(terrainPos.x, terrainPos.y + treeHeight, terrainPos.z));
	//snowEmitter.setEmitSpeed(0.1f);
    snowEmitter.setEmitSpeed(0.02f);
	snowEmitter.setEmitSpeedRange(0.005f);
	snowEmitter.setEmitDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	snowEmitter.setEmitAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
	snowEmitter.setEmitInterval(0.01f);
	snowEmitter.setParticleLife(20.0f);
    Circle snowEmitShape(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, terrainRadius);
	//Circle snowEmitShape(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, treeBaseRadius);
	snowEmitter.setEmitShape(&snowEmitShape);

    // lightParticle Emitter
	Particle light(&lightModelWrap);
	light.setDecay(true);
    light.setDecayRate(2.5f);
	light.setFade(true);
	//ParticleEmitter lightEmitter(&physicsSystem, snow, treePos + glm::vec3(0.0f, treeHeight, 0.0f));
	ParticleEmitter lightEmitter(&physicsSystem, light, treePos + glm::vec3(0.0f, treeHeight - 0.003f, 0.0f));
    lightEmitter.setEmitCount(100);
    lightEmitter.setEmitSpeed(0.01f);
	//lightEmitter.setEmitDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	//lightEmitter.setEmitAcceleration(glm::vec3(0.0f, -9.8f / 1000, 0.0f));
	lightEmitter.setEmitAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
	lightEmitter.setEmitInterval(0.02f);
	lightEmitter.setParticleLife(1.5f);
    //lightEmitter.setParticleLife(2.0f);
	Point lightEmitShape(glm::vec3(0.0f));
	lightEmitter.setEmitShape(&lightEmitShape);
	lightEmitter.setRandomEmitDirection(true);
    
    RigidBody terrainRigidBody(&terrainModelWrap);
	terrainRigidBody.setPosition(terrainBasePos);
	terrainRigidBody.setPhysicsSystem(&physicsSystem);
    terrainRigidBody.addCollisionLayer(1);
    terrainRigidBody.setCollisionBox(new Cylinder(terrainShape));

	RigidBody treeRigidBody(&treeModelWrap);
	treeRigidBody.setPosition(treePos);
	treeRigidBody.setPhysicsSystem(&physicsSystem);
	treeRigidBody.addCollisionLayer(1);
    treeRigidBody.setCollisionBox(new Cone(treeShape));

	shared_ptr<RigidBody> treeRigidBodyPtr = make_shared<RigidBody>(treeRigidBody);
	shared_ptr<RigidBody> terrainRigidBodyPtr = make_shared<RigidBody>(terrainRigidBody);

	physicsSystem.AddRigidBody(treeRigidBodyPtr);
	physicsSystem.AddRigidBody(terrainRigidBodyPtr);

    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 时间逻辑
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 输入
        // -----
        processInput(window);

        if (isTreeLit) {
            lightEmitter.emit(deltaTime);
        }

        treeRigidBody.model->setLight(isTreeLit);

        // 开始渲染
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 物理
		// ------
		physicsSystem.updateAll(deltaTime, camera);
		if (emitSnow)
		    snowEmitter.emit(deltaTime);

        /*if (emitLight) {
            lightEmitter.emit(deltaTime);
            emitLight = false;
        }*/

        // 确保在设置 Uniforms/Drawing 对象时激活 Shader
        //---------------------------------------------------------------------
        
		roomModelWrap.draw(camera);
		
		blackboardModelWrap.draw(camera);

		lightCubeModelWrap.draw(camera);

		//starModelWrap.draw(camera);


		// 绘制风车
        if (showWindmill) {
            // 设置颜色
			windmillModelWrap.setColor(windmillColor);

            if (windmillRotate) 
				windmillModelWrap.rotate(windmillRotateSpeed * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

			windmillModelWrap.draw(camera);
        }

		tableModelWrap.draw(camera);
   
        // glfw：交换缓冲区和轮询 IO 事件（按下/释放键、移动鼠标等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw：终止，清除所有以前分配的 GLFW 资源。
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// 记录按键的上一次状态
bool lastKeyCState = false;
bool lastKeySState = false;
bool lastKeyEState = false;
bool lastKeyLState = false;
//查询 GLFW 是否按下/释放了该帧的相关键并做出相应的反应
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);*/

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
		if (!showWindmill) 
			showWindmill = true;
	}
    // 获取当前按键状态
    bool currentKeyCState = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    bool currentKeySState = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	bool currentKeyEState = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
	bool currentKeyLState = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;

    // 检查按键 C 是否从未按下变为按下
    if (currentKeyCState && !lastKeyCState) {
        // 将风车的颜色设置为随机颜色
        windmillColor = glm::vec3(
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX
        );
    }

    // 检查按键 S 是否从未按下变为按下
    if (currentKeySState && !lastKeySState) {
        // 切换风车的旋转状态
        windmillRotate = !windmillRotate;
    }

	// 检查按键 W 是否从未按下变为按下
    if (currentKeyEState && !lastKeyEState) {
        // 下雪
        emitSnow = !emitSnow;
    }

	// 检查按键 L 是否从未按下变为按下
	if (currentKeyLState && !lastKeyLState) {
		// 切换光照
		isTreeLit = !isTreeLit;
	}

    // 更新按键的上一次状态
    lastKeyCState = currentKeyCState;
    lastKeySState = currentKeySState;
	lastKeyEState = currentKeyEState;
	lastKeyLState = currentKeyLState;
}

// glfw：每当窗口大小发生变化（通过操作系统或用户调整大小）时，此回调函数都会执行
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保视区与新的窗口尺寸匹配;请注意，width和height将明显大于 Retina 显示屏上指定的高度
    glViewport(0, 0, width, height);
}


// glfw: 每当鼠标移动时，该回调都会被调用
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 反转，因为 y 坐标从下到上

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw:每当鼠标滚轮滚动时，该回调都会被调用
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}





