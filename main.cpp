#include "include/mymodelwrap.h"

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

    // ��������
	initData();

    // ����shader����
    Shader lightingShader("resources/shader/arealighting.vs", "resources/shader/arealighting.fs");
    Shader lightCubeShader("resources/shader/lightcube.vs", "resources/shader/lightcube.fs");

	/*MyMesh FWall(FWallVertices, FWallVerticesSize, FWallColor);
	MyMesh LWall(LWallVertices, LWallVerticesSize, LWallColor);
	MyMesh RWall(RWallVertices, RWallVerticesSize, RWallColor);
	MyMesh Ceiling(CeilingVertices, CeilingVerticesSize, CeilingColor);
	MyMesh Floor(FloorVertices, FloorVerticesSize, FloorColor);*/

	MyMesh FWall(FWallVertices, FWallVerticesSize, FWallMaterial);
	MyMesh LWall(LWallVertices, LWallVerticesSize, LWallMaterial);
	MyMesh RWall(RWallVertices, RWallVerticesSize, RWallMaterial);
	MyMesh Ceiling(CeilingVertices, CeilingVerticesSize, CeilingMaterial);
	MyMesh Floor(FloorVertices, FloorVerticesSize, FloorMaterial);
	
	MyMesh lightCube(areaLightVertices, areaLightVerticesSize, lightCubeColor);

    // �ܵ�ģ��
	MyMesh bezierMesh(bezierVerticesData, FWallMaterial);

	std::vector<MyMesh*> room = { &FWall, &LWall, &RWall, &Ceiling, &Floor};

	MyModel roomModel(room);
	MyModel lightCubeModel({ &lightCube });
	MyModel bezierModel({ &bezierMesh });

	MyModelWrap roomModelWrap(&roomModel, &lightingShader, roomGeometry);
    MyModelWrap lightCubeModelWrap(&lightCubeModel, &lightCubeShader, areaLightGeometry);
	MyModelWrap bezierModelWrap(&bezierModel, &lightingShader, bezierGeometry);

	// ���Դ
	GLuint ssbo; // �� Shader �洢�������
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, samplePoints.size() * sizeof(glm::vec3), samplePoints.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // �� SSBO �󶨵��󶨵� 0


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

        // ��ʼ��Ⱦ
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ȷ�������� Uniforms/Drawing ����ʱ���� Shader
        //---------------------------------------------------------------------
        
		roomModelWrap.draw(camera);
		lightCubeModelWrap.draw(camera);
		bezierModelWrap.draw(camera);
   
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

//��ѯ GLFW �Ƿ���/�ͷ��˸�֡����ؼ���������Ӧ�ķ�Ӧ
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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





