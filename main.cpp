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

// ʱ������
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // ��ʼ��������glfw
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw��������
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

    // ���� GLFW �������ǵ����
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad���������� OpenGL ����ָ��
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ����ȫ�� OpenGL ״̬
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // ���ɵ���
	Terrain terrain(terrainRadius, terrainBaseHeight + terrainMinHeight, terrainBaseHeight + terrainMaxHeight, terrainRoughness, terrainTextureSize);
	terrain.setVerticesArray();
	float* terrainVertices = terrain.getVerticesArray();
	unsigned int terrainVerticesSize = terrain.getVerticesArraySize();

    // ��������
	initData();

    // ����shader����
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
    // �ʵ�
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

	// ��������ϵͳ

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

    // ��Ⱦѭ��
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // ʱ���߼�
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ����
        // -----
        processInput(window);

        if (isTreeLit) {
            lightEmitter.emit(deltaTime);
        }

        treeRigidBody.model->setLight(isTreeLit);

        // ��ʼ��Ⱦ
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ����
		// ------
		physicsSystem.updateAll(deltaTime, camera);
		if (emitSnow)
		    snowEmitter.emit(deltaTime);

        /*if (emitLight) {
            lightEmitter.emit(deltaTime);
            emitLight = false;
        }*/

        // ȷ�������� Uniforms/Drawing ����ʱ���� Shader
        //---------------------------------------------------------------------
        
		roomModelWrap.draw(camera);
		
		blackboardModelWrap.draw(camera);

		lightCubeModelWrap.draw(camera);

		//starModelWrap.draw(camera);


		// ���Ʒ糵
        if (showWindmill) {
            // ������ɫ
			windmillModelWrap.setColor(windmillColor);

            if (windmillRotate) 
				windmillModelWrap.rotate(windmillRotateSpeed * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

			windmillModelWrap.draw(camera);
        }

		tableModelWrap.draw(camera);
   
        // glfw����������������ѯ IO �¼�������/�ͷż����ƶ����ȣ�
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw����ֹ�����������ǰ����� GLFW ��Դ��
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// ��¼��������һ��״̬
bool lastKeyCState = false;
bool lastKeySState = false;
bool lastKeyEState = false;
bool lastKeyLState = false;
//��ѯ GLFW �Ƿ���/�ͷ��˸�֡����ؼ���������Ӧ�ķ�Ӧ
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
    // ��ȡ��ǰ����״̬
    bool currentKeyCState = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    bool currentKeySState = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	bool currentKeyEState = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
	bool currentKeyLState = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;

    // ��鰴�� C �Ƿ��δ���±�Ϊ����
    if (currentKeyCState && !lastKeyCState) {
        // ���糵����ɫ����Ϊ�����ɫ
        windmillColor = glm::vec3(
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX
        );
    }

    // ��鰴�� S �Ƿ��δ���±�Ϊ����
    if (currentKeySState && !lastKeySState) {
        // �л��糵����ת״̬
        windmillRotate = !windmillRotate;
    }

	// ��鰴�� W �Ƿ��δ���±�Ϊ����
    if (currentKeyEState && !lastKeyEState) {
        // ��ѩ
        emitSnow = !emitSnow;
    }

	// ��鰴�� L �Ƿ��δ���±�Ϊ����
	if (currentKeyLState && !lastKeyLState) {
		// �л�����
		isTreeLit = !isTreeLit;
	}

    // ���°�������һ��״̬
    lastKeyCState = currentKeyCState;
    lastKeySState = currentKeySState;
	lastKeyEState = currentKeyEState;
	lastKeyLState = currentKeyLState;
}

// glfw��ÿ�����ڴ�С�����仯��ͨ������ϵͳ���û�������С��ʱ���˻ص���������ִ��
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ȷ���������µĴ��ڳߴ�ƥ��;��ע�⣬width��height�����Դ��� Retina ��ʾ����ָ���ĸ߶�
    glViewport(0, 0, width, height);
}


// glfw: ÿ������ƶ�ʱ���ûص����ᱻ����
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
    float yoffset = lastY - ypos; // ��ת����Ϊ y ������µ���

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw:ÿ�������ֹ���ʱ���ûص����ᱻ����
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}





