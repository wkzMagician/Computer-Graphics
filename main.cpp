#include "include/mymodelwrap.h"

#include <GLFW/glfw3.h>

#include <iostream>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


bool firstMouse = true;

// 时间设置
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool isShowMenu = false;
bool isShowNormal = false;
bool isShowPipe = true;

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

    // 加载数据
	initData();

    // 编译shader操作
    Shader lightingShader("resources/shader/arealighting.vs", "resources/shader/arealighting.fs");
    Shader lightCubeShader("resources/shader/lightcube.vs", "resources/shader/lightcube.fs");
	Shader curveShader("resources/shader/curve.vs", "resources/shader/curve.fs");
    // 可视化法线Shader
	Shader normalShader("resources/shader/normal.vs", "resources/shader/normal.fs", "resources/shader/normal.gs");

    // 天空盒
    Shader skyboxShader("resources/shader/skybox.vs", "resources/shader/skybox.fs");
	skyboxShader.setInt("skybox", 0);

	MyMesh FWall(FWallVertices, FWallVerticesSize, FWallMaterial);
	MyMesh LWall(LWallVertices, LWallVerticesSize, LWallMaterial);
	MyMesh RWall(RWallVertices, RWallVerticesSize, RWallMaterial);
	MyMesh Ceiling(CeilingVertices, CeilingVerticesSize, CeilingMaterial);
	MyMesh Floor(FloorVertices, FloorVerticesSize, FloorMaterial);
	
	MyMesh lightCube(areaLightVertices, areaLightVerticesSize, lightCubeColor);

    // 管道模型
	MyMesh pipeMesh(pipeVerticesData, pipeMaterial);

    // 关键线
    MyMesh rectMesh(rectVertices, pipeMaterial);
	MyMesh ellipseMesh(ellipseVertices, pipeMaterial);
	MyMesh circleMesh(circleVertices, pipeMaterial);
    rectMesh.setDrawLine(true, true);
	ellipseMesh.setDrawLine(true, true);
	circleMesh.setDrawLine(true, true);

	MyMesh bezierMesh(bezierVerticesData, pipeMaterial);
	bezierMesh.setDrawLine(true, false);

    MyMesh skyboxMesh(skyboxVertices, skyboxVerticesSize, faces, MyMesh::CUBE_MAP, 3);

	std::vector<MyMesh*> room = { &FWall, &LWall, &RWall, &Ceiling, &Floor};

	MyModel roomModel(room);
	MyModel lightCubeModel({ &lightCube });
	MyModel pipeModel({ &pipeMesh });
	MyModel bezierModel({ &bezierMesh, &rectMesh, &ellipseMesh, &circleMesh });
    MyModel skyboxModel({ &skyboxMesh });

	MyModelWrap roomModelWrap(&roomModel, &lightingShader, roomGeometry);
    MyModelWrap lightCubeModelWrap(&lightCubeModel, &lightCubeShader, areaLightGeometry);
	MyModelWrap pipeModelWrap(&pipeModel, &lightingShader, bezierGeometry);
	
    MyModelWrap bezierModelWrap(&bezierModel, &curveShader, bezierGeometry);
	MyModelWrap normalModelWrap(&pipeModel, &normalShader, bezierGeometry);
    
    MyModelWrap skyboxModelWrap(&skyboxModel, &skyboxShader);

	// 面光源
	GLuint ssbo; // 绑定 Shader 存储缓冲对象
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, samplePoints.size() * sizeof(glm::vec3), samplePoints.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // 将 SSBO 绑定到绑定点 0

    // IMGUI
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

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

        // imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(500, 300)); // 设置窗口大小为400x300
		ImGui::Begin(u8"控制窗口(按 M to 切换菜单模式和场景模式)");

        if (ImGui::Button(u8"退出", ImVec2(60, 20))) { // 设置按钮大小为100x50 
            glfwSetWindowShouldClose(window, true);
        }

		if (isShowNormal) {
			if(ImGui::Button(u8"取消显示法线", ImVec2(100, 20))){
				isShowNormal = false;
			}
		} else {
			if (ImGui::Button(u8"显示法线", ImVec2(100, 20))) {
				isShowNormal = true;
			}
		}

		if (isShowPipe) {
			if (ImGui::Button(u8"取消显示管道", ImVec2(100, 20))) {
				isShowPipe = false;
			}
		} else {
			if (ImGui::Button(u8"显示管道", ImVec2(100, 20))) {
				isShowPipe = true;
			}   
		}

        // 输入框
		ImGui::Text(u8"管道材质");
		ImGui::SliderFloat(u8"管道：环境光遮蔽", &pipeAmbientOcclusion, 0.0f, 1.0f);
		ImGui::SliderFloat(u8"管道：金属度", &pipeMetallic, 0.0f, 1.0f);
		ImGui::SliderFloat(u8"管道：粗糙度", &pipeRoughness, 0.0f, 1.0f);
		ImGui::ColorEdit3(u8"管道：颜色", (float*)&pipeColor);
        ImGui::End();

		pipeMesh.setMaterial(pipeColor, pipeMetallic, pipeRoughness, pipeAmbientOcclusion);

        // 开始渲染
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 确保在设置 Uniforms/Drawing 对象时激活 Shader
        //---------------------------------------------------------------------
        
		roomModelWrap.draw(camera);
		lightCubeModelWrap.draw(camera);
		if(isShowPipe) pipeModelWrap.draw(camera);
		bezierModelWrap.draw(camera);
        if (isShowNormal) normalModelWrap.draw(camera);

		// 绘制天空盒
		glDepthFunc(GL_LEQUAL);  // 更改深度函数，以便深度测试在值等于深度缓冲区最大值时通过
		skyboxModelWrap.draw(camera);
		glDepthFunc(GL_LESS); // 恢复正常深度函数

		// 绘制imgui
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   
        // glfw：交换缓冲区和轮询 IO 事件（按下/释放键、移动鼠标等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw：终止，清除所有以前分配的 GLFW 资源。
    // ------------------------------------------------------------------
    glfwTerminate();

	// 释放资源
	// ------------------------------------------------------------------
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

    return 0;
}

//查询 GLFW 是否按下/释放了该帧的相关键并做出相应的反应
// ---------------------------------------------------------------------------------------------------------
bool is_M_Pressed = false;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 退出
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (!is_M_Pressed) {
            is_M_Pressed = true;
            isShowMenu = !isShowMenu;
        }
        if (isShowMenu) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 显示鼠标光标
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标光标
        }
    }else{
		if (is_M_Pressed) is_M_Pressed = false;
	}

	if (isShowMenu) return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
	if (isShowMenu) return;

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
	if (isShowMenu) return;

    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}





