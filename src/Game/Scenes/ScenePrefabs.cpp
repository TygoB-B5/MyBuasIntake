// Tygo Boons 2022

#include "mypch.h"
#include "ScenePrefabs.h"

namespace Gameplay
{
	RefPtr<Renderer::Shader> ScenePrefabs::s_Basic3DShader = nullptr;
	RefPtr<Renderer::Texture> ScenePrefabs::s_TextureAtlasTexture = nullptr;
	RefPtr<Renderer::Material> ScenePrefabs::s_TextureAtlasMaterial = nullptr;

	void ScenePrefabs::Initialize()
	{
		// Initialize static variables.
		s_Basic3DShader = Renderer::Shader::Create("Assets/Shaders/Basic3DShader.glsl");
		s_TextureAtlasTexture = Renderer::Texture::Create("Assets/Textures/TextureAtlas.png");
		s_TextureAtlasMaterial = Renderer::Material::Create(s_TextureAtlasTexture, s_Basic3DShader);
	}

	std::vector<RefPtr<Entity::GameObject>> ScenePrefabs::CreateEnviroment()
	{
		// Create vector that has to be filled.
		std::vector<RefPtr<Entity::GameObject>> objects(5);

		/// Left collider.
		{
			RefPtr<Entity::GameObject> barrier = Entity::GameObject::Create("LeftBarrier");
			RefPtr<Entity::Transform> transform = barrier->GetComponent<Entity::Transform>();
			auto boxCollider = barrier->AddComponent(Entity::BoxCollider::Create());

			transform->SetScale({ 80, 5, 1 });
			transform->SetPosition({ 0, 1, -22 });

			boxCollider->SetSize(transform->GetScale());

			objects[0] = barrier;
		}

		/// Right collider
		{
			RefPtr<Entity::GameObject> barrier = Entity::GameObject::Create("RightBarrier");
			RefPtr<Entity::Transform> transform = barrier->GetComponent<Entity::Transform>();
			auto boxCollider = barrier->AddComponent(Entity::BoxCollider::Create());

			transform->SetScale({ 80, 5, 1 });
			transform->SetPosition({ 0, 1, 21.5f });

			boxCollider->SetSize(transform->GetScale());

			objects[1] = barrier;
		}

		/// Bottom collider.
		{
			RefPtr<Entity::GameObject> barrier = Entity::GameObject::Create("BottomBarrier");
			RefPtr<Entity::Transform> transform = barrier->GetComponent<Entity::Transform>();
			auto boxCollider = barrier->AddComponent(Entity::BoxCollider::Create());

			transform->SetScale({ 1, 5, 80 });
			transform->SetPosition({ -14.5f, 1, 0 });

			boxCollider->SetSize(transform->GetScale());

			objects[2] = barrier;
		}

		/// Top collider.
		{
			RefPtr<Entity::GameObject> barrier = Entity::GameObject::Create("TopBarrier");
			RefPtr<Entity::Transform> transform = barrier->GetComponent<Entity::Transform>();
			auto boxCollider = barrier->AddComponent(Entity::BoxCollider::Create());

			transform->SetScale({ 1, 5, 80 });
			transform->SetPosition({ 15.25f, 1, 0 });

			boxCollider->SetSize(transform->GetScale());

			objects[3] = barrier;
		}

		{
			/// Enviroment mesh.
			RefPtr<Entity::GameObject> enviroment = Entity::GameObject::Create("Enviroment");

			enviroment->AddComponent(Entity::MeshRenderer::Create());
			enviroment->GetComponent<Entity::MeshRenderer>()->SetMesh(Renderer::Mesh::Create("Assets/Models/Enviroment.obj"));
			enviroment->GetComponent<Entity::MeshRenderer>()->SetMaterial(s_TextureAtlasMaterial);

			// Lower enviroment a little to prevent z-fighting.
			enviroment->GetComponent<Entity::Transform>()->SetPosition({ 0, -0.1f, 0 });

			objects[4] = enviroment;
		}

		return objects;
	}

	RefPtr<Entity::GameObject> ScenePrefabs::CreateSceneCamera()
	{
		// Create camera object.
		RefPtr<Entity::GameObject> camera = Entity::GameObject::Create("MainCamera");

		RefPtr<Entity::Camera> cam = camera->AddComponent(Entity::Camera::Create());

		// Set view properties, position and rotation to match enviroment.
		cam->SetOrtographicProjection(18.5f, 0.001f, 100.0f);
		cam->SetViewPoint({ -33.5f, 40.0f, -7.2f });
		cam->SetViewRotation({ -45, -90, 0 });

		return camera;
	}

	std::vector<RefPtr<Entity::GameObject>> ScenePrefabs::CreatePlayerTank(const glm::vec2& position)
	{
		// Create vector that has to be filled.
		std::vector<RefPtr<Entity::GameObject>> objects(2);


		/// Create tank body object
		RefPtr<Entity::GameObject> tankBody = Entity::GameObject::Create("PlayerTank");

		// Set tank position.
		tankBody->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0, position.y });

		/// Rendering components.
		auto bodyRenderer = tankBody->AddComponent(Entity::MeshRenderer::Create());
		bodyRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankBodyPlayer.obj"));
		bodyRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/Player/tank_blue.png"),
			s_Basic3DShader
		));

		// Create tank shadow
		auto shadowRenderer = tankBody->AddComponent(Entity::MeshRenderer::Create());
		shadowRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankShadow.obj"));
		shadowRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/tank_shadow.png"),
			s_Basic3DShader
		));
		/// -

		/// Collision components.
		// Add collision components.
		auto tankCollider = tankBody->AddComponent(Entity::BoxCollider::Create());
		tankCollider->SetSize({ 2.0f, 5, 2.0f });

		tankBody->AddComponent(Entity::PhysicsBody::Create());
		/// -

		/// Behaviour
		// Add tank movement component.
		RefPtr<Gameplay::TankEngine> tankEngine = tankBody->AddComponent(Gameplay::TankEngine::Create());
		// Add Player input.
		tankBody->AddComponent(Gameplay::TankInputPlayer::Create());

		// Add track audio.
		tankBody->AddComponent(TankTrackSound::Create());
		/// -

		/// Audio
		// Add tank explosions
		RefPtr<Entity::AudioSource> explosionAudio = tankBody->AddComponent(Entity::AudioSource::Create());
		explosionAudio->LoadClipFromFile("Assets/Audio/Effects/Explosion.wav");
		explosionAudio->SetVolume(0.5f);
		/// - 



		/// Create tank gun object.
		RefPtr<Entity::GameObject> tankGun = Entity::GameObject::Create("EnemyTankGun");

		// Set tank gun position.
		tankGun->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0, position.y });

		// Create enemy run rendering.
		auto gunRenderer = tankGun->AddComponent(Entity::MeshRenderer::Create());
		gunRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankGunPlayer.obj"));
		gunRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/Player/tank_blue.png"),
			s_Basic3DShader
		));
		/// -

		/// Behaviour
		// Add behaviour component and attach theh tank body to the tank gun.
		RefPtr<Gameplay::TankGun> gunBehaviour = tankGun->AddComponent(Gameplay::TankGun::Create());
		gunBehaviour->SetTankParent(tankBody->GetComponent<Entity::Transform>());
		/// -

		/// Audio
		// Add gun audio.
		RefPtr<Entity::AudioSource> shootSound = tankGun->AddComponent(Entity::AudioSource::Create());
		shootSound->LoadClipFromFile("Assets/Audio/Effects/Thud.wav");
		shootSound->SetVolume(0.3f);
		///  - 


		// Set tank gun reference in tank engine.
		tankEngine->SetGunObject(gunBehaviour);


		objects[0] = tankBody;
		objects[1] = tankGun;

		return objects;
	}

	RefPtr<Entity::GameObject> ScenePrefabs::CreateSpectatorCamera()
	{
		// Create debug spectator cam.
		RefPtr<Entity::GameObject> flycam = Entity::GameObject::Create("SpectatorCamera");

		flycam->AddComponent(Entity::Camera::Create());
		flycam->AddComponent(Gameplay::FlyingCamera::Create());

		return flycam;
	}

	std::vector<RefPtr<Entity::GameObject>> ScenePrefabs::CreateBrownEnemyTank(const glm::vec2& position)
	{
		// Create vector that has to be filled.
		std::vector<RefPtr<Entity::GameObject>> objects(2);


		/// Create tank body object
		RefPtr<Entity::GameObject> tankBody = Entity::GameObject::Create("EnemyTank");

		// Set tank position.
		tankBody->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0, position.y });

		/// Rendering components.
		auto bodyRenderer = tankBody->AddComponent(Entity::MeshRenderer::Create());
		bodyRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankBodyEnemy.obj"));
		bodyRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/Enemy/tank_brown.png"),
			s_Basic3DShader
		));

		// Create tank shadow
		auto shadowRenderer = tankBody->AddComponent(Entity::MeshRenderer::Create());
		shadowRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankShadow.obj"));
		shadowRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/tank_shadow.png"),
			s_Basic3DShader
		));
		/// -

		/// Collision components.
		// Add collision components.
		auto tankCollider = tankBody->AddComponent(Entity::BoxCollider::Create());
		tankCollider->SetSize({ 2.0f, 5, 2.0f });

		tankBody->AddComponent(Entity::PhysicsBody::Create());
		/// -

		/// Behaviour
		// Add tank movement component.
		RefPtr<Gameplay::TankEngine> tankEngine = tankBody->AddComponent(Gameplay::TankEngine::Create());

		// Add AI input.
		tankBody->AddComponent(Gameplay::TankInputBrownEnemy::Create());
		/// -

		/// Create tank gun object.
		RefPtr<Entity::GameObject> tankGun = Entity::GameObject::Create("EnemyTankGun");

		// Set tank gun position.
		tankGun->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0, position.y });

		// Create enemy run rendering.
		auto gunRenderer = tankGun->AddComponent(Entity::MeshRenderer::Create());
		gunRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankGunEnemy.obj"));
		gunRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/Enemy/tank_brown.png"),
			s_Basic3DShader
		));
		/// -

		/// Behaviour
		// Add behaviour component and attach theh tank body to the tank gun.
		RefPtr<Gameplay::TankGun> gunBehaviour = tankGun->AddComponent(Gameplay::TankGun::Create());
		gunBehaviour->SetTankParent(tankBody->GetComponent<Entity::Transform>());
		/// -

		/// Audio
		// Add track audio.
		RefPtr<TankTrackSound> trackSound = tankBody->AddComponent(TankTrackSound::Create());
		trackSound->SetPitch(1.2f);
		///  - 


		// Set tank gun reference in tank engine.
		tankEngine->SetGunObject(gunBehaviour);

		objects[0] = tankBody;
		objects[1] = tankGun;

		return objects;
	}

	std::vector<RefPtr<Entity::GameObject>> ScenePrefabs::CreateAshEnemyTank(const glm::vec2& position)
	{
		// Create vector that has to be filled.
		std::vector<RefPtr<Entity::GameObject>> objects(2);


		/// Create tank body object
		RefPtr<Entity::GameObject> tankBody = Entity::GameObject::Create("EnemyTank");

		// Set tank position.
		tankBody->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0, position.y });

		/// Rendering components.
		auto bodyRenderer = tankBody->AddComponent(Entity::MeshRenderer::Create());
		bodyRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankBodyEnemy.obj"));
		bodyRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/Enemy/tank_ash.png"),
			s_Basic3DShader
		));

		// Create tank shadow
		auto shadowRenderer = tankBody->AddComponent(Entity::MeshRenderer::Create());
		shadowRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankShadow.obj"));
		shadowRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/tank_shadow.png"),
			s_Basic3DShader
		));
		/// -

		/// Collision components.
		// Add collision components.
		auto tankCollider = tankBody->AddComponent(Entity::BoxCollider::Create());
		tankCollider->SetSize({ 2.0f, 5, 2.0f });

		tankBody->AddComponent(Entity::PhysicsBody::Create());
		/// -

		/// Behaviour
		// Add tank movement component.
		RefPtr<Gameplay::TankEngine> tankEngine = tankBody->AddComponent(Gameplay::TankEngine::Create());

		// Add AI input.
		tankBody->AddComponent(Gameplay::TankInputAshEnemy::Create());
		/// -

		/// Create tank gun object.
		RefPtr<Entity::GameObject> tankGun = Entity::GameObject::Create("EnemyTankGun");

		// Set tank gun position.
		tankGun->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0, position.y });

		// Create enemy run rendering.
		auto gunRenderer = tankGun->AddComponent(Entity::MeshRenderer::Create());
		gunRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Tanks/TankGunEnemy.obj"));
		gunRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Tanks/Enemy/tank_ash.png"),
			s_Basic3DShader
		));
		/// -

		/// Behaviour
		// Add behaviour component and attach theh tank body to the tank gun.
		RefPtr<Gameplay::TankGun> gunBehaviour = tankGun->AddComponent(Gameplay::TankGun::Create());
		gunBehaviour->SetTankParent(tankBody->GetComponent<Entity::Transform>());
		/// -

		/// Audio
		// Add track audio.
		RefPtr<TankTrackSound> trackSound = tankBody->AddComponent(TankTrackSound::Create());
		trackSound->SetPitch(1.2f);
		///  - 


		// Set tank gun reference in tank engine.
		tankEngine->SetGunObject(gunBehaviour);

		objects[0] = tankBody;
		objects[1] = tankGun;

		return objects;
	}

	RefPtr<Entity::GameObject> ScenePrefabs::CreateBullet()
	{
		/// Create bullet game object.
		RefPtr<Entity::GameObject> bullet = Entity::GameObject::Create("Bullet");

		RefPtr<Entity::MeshRenderer> bodyRenderer = bullet->AddComponent(Entity::MeshRenderer::Create());
		bodyRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Bullet.obj"));
		bodyRenderer->SetMaterial(Renderer::Material::Create(
			Renderer::Texture::Create("Assets/Textures/Bullet.png"),
			s_Basic3DShader
		));

		// Set the size of the bullet collider.
		RefPtr<Entity::BoxCollider> boxCollider = bullet->AddComponent(Entity::BoxCollider::Create());
		boxCollider->SetSize({ 0.4f, 1, 0.4f });

		// Add a physicsbody component so it gets oncollisionenter events.
		bullet->AddComponent(Entity::PhysicsBody::Create());

		// Set the bullet scale.
		bullet->GetComponent<Entity::Transform>()->SetScale({ 0.075f, 0.075f, 0.075f });

		bullet->AddComponent(Bullet::Create());

		return bullet;
	}

	RefPtr<Entity::GameObject> ScenePrefabs::CreateBlockMesh(const glm::vec2& position)
	{
		/// Create block game object.
		RefPtr<Entity::GameObject> block = Entity::GameObject::Create("Block");

		auto blockRenderer = block->AddComponent(Entity::MeshRenderer::Create());
		blockRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Block.obj"));
		blockRenderer->SetMaterial(s_TextureAtlasMaterial);

		// Set block position to defined position and slightly increase the Y axis depending on the x position to prevent z clipping.
		block->GetComponent<Entity::Transform>()->SetPosition({ position.x, position.x * 0.001f, position.y });

		return block;
	}

	RefPtr<Entity::GameObject> ScenePrefabs::CreateCollider(const glm::vec2& position, const glm::vec2& scale, bool enableDebug)
	{
		/// Create collider game object.
		RefPtr<Entity::GameObject> collider = Entity::GameObject::Create("Collider");

		// Add collision components.
		auto col = collider->AddComponent(Entity::BoxCollider::Create());
		col->GetComponent<Entity::Transform>()->SetPosition({ position.x, 0.0f, position.y });
		col->SetSize({ scale.x, 5, scale.y });


		/// Create debug mesh
		if (enableDebug)
		{
			auto shadowRenderer = collider->AddComponent(Entity::MeshRenderer::Create());
			shadowRenderer->SetMesh(Renderer::Mesh::Create("Assets/Models/Cube.obj"));
			shadowRenderer->SetMaterial(Renderer::Material::Create(
				Renderer::Texture::Create("Assets/Textures/Test.png"),
				s_Basic3DShader
			));

			collider->GetComponent<Entity::Transform>()->SetScale({ scale.x, 1, scale.y });
		}

		return collider;
	}

	RefPtr<Entity::GameObject> ScenePrefabs::CreateTankManager(uint32_t amountOfEnemyTanksInLevel)
	{
		/// Create tank manager game object.
		RefPtr<Entity::GameObject> tankManager = Entity::GameObject::Create("TankManager");

		// Add tank manager behaviour component.
		auto tankManagerBehaviour = tankManager->AddComponent(TankManager::Create());

		// Set amount of enemy tanks in scene.
		tankManagerBehaviour->SetEnemyCount(amountOfEnemyTanksInLevel);

		return tankManager;
	}
	RefPtr<Entity::GameObject> ScenePrefabs::CreateMusic(std::string filepath)
	{
		RefPtr<Entity::GameObject> mainTheme = Entity::GameObject::Create("Music");

		// Create audio source and play it.
		auto aud = mainTheme->AddComponent(Entity::AudioSource::Create());
		aud->LoadClipFromFile(filepath);
		aud->SetLooping(true);
		aud->Play();

		return mainTheme;
	}
}