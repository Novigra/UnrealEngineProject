# UnrealEngineProject
 
## Rock, Paper, Scissors, Shoot! 🔫 (WIP)


# The Game Design

---

In this project, the player partakes in a game where they play two matches: the first is a rock, paper, scissors match, and the second is a fight match.

The rock, paper, scissors match consists of three rounds. Whoever wins gets strong abilities, while the other player or AI gets weaker ones. The winner gets a shotgun with a low fire rate and a full health bar, the loser gets a rifle with a high fire rate and a half health bar, and both players will have different abilities.

The idea of the second match is to give the loser a second chance to win against the other player, but we have to keep in mind that it will be hard for the losing party to win it, and they have a slight chance to win.

Note: I’m not an animator, so the animation is going to look rough..😭

![RPSMatch](https://github.com/Novigra/UnrealEngineProject/assets/59344266/05074860-27c0-40b7-9d46-101f2cfaefd7)

# Rock, Paper, Scissor Match

---

![Match](https://github.com/Novigra/UnrealEngineProject/assets/59344266/f9443092-ca7b-4baf-a3ba-b7efd8912ba0)


In the first match, you play a three round rock, paper, scissors match. The way it was implemented first, I defined what is the player’s status, which is the most important part as it defines the actions that the player can take. It can also help us with animation transitions. 😉

```
// Player Status depends on whether the player is in the (Rock, Paper, Scissors) match or in the fight.
UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	EPS_Match		UMETA(DisplayName = "Match"),
	EPS_Pending		UMETA(DisplayName = "Pending"),
	EPS_Fight		UMETA(DisplayName = "Fight")
};

UENUM(BlueprintType)
enum class EPlayerChoice : uint8
{
	EPC_Rock		UMETA(DisplayName = "Rock"),
	EPC_Paper		UMETA(DisplayName = "Paper"),
	EPC_Scissors	UMETA(DisplayName = "Scissors"),

	EPC_NONE		UMETA(DisplayName = "NONE")
};

// Result Status depends on whether the player won the match or not.
UENUM(BlueprintType)
enum class EPlayerResult : uint8
{
	EPR_Winner	UMETA(DisplayName = "Winner"),
	EPR_Loser	UMETA(DisplayName = "Loser"),

	EPR_NONE	UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EPlayerAnimTrans : uint8
{
	EPAT_Choosing	UMETA(DisplayName = "Choosing"),
	EPAT_Playing	UMETA(DisplayName = "Playing"),

	EPAT_NONE	UMETA(DisplayName = "NONE")
};
```
| MyPlayerBP                                                                                                       | Rules and status                                                                                                       |
| ---------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------- |
| ![PlayerBP](https://github.com/Novigra/UnrealEngineProject/assets/59344266/5909dcb6-35f1-4f1c-9144-e47bdfcd4396) | ![PlayerBP_ENUMS](https://github.com/Novigra/UnrealEngineProject/assets/59344266/2b1fcfb9-5aad-4474-89c1-94ae3170e17d) |

To give a player a way to play, we need visual elements, and to achieve this, we use the amazing Unreal Engine tool UMG. I designed buttons and implemented some functionality using blueprint.

![RPS_Buttons](https://github.com/Novigra/UnrealEngineProject/assets/59344266/2a915156-83ab-421e-b8ac-772bc21ddaa7)

Rock, Paper, Scissors Implementation

![RPS_Implementation](https://github.com/Novigra/UnrealEngineProject/assets/59344266/10298c21-0805-4db8-864c-a86688be93a3)

Closer Look

![RPS_Implementation(CloserLook)](https://github.com/Novigra/UnrealEngineProject/assets/59344266/b8e42c0f-3aea-4869-bb29-a5cf8ace4975)

There’s something to keep in mind, which is the player controller. Normally, when you switch between UI and gameplay, you need to set the player’s input mode to ensure that the player controls don’t go all over the place.
```
// UI
PlayerController->SetInputMode(FInputModeUIOnly());

// Game
PlayerController->SetInputMode(FInputModeGameOnly());
```
Of course we can’t forget our animations, no player would like looking at a t-pose for several hours. I did some jaw dropping animations 💀, so I can implement them in the animation blueprint.

![AnimContent](https://github.com/Novigra/UnrealEngineProject/assets/59344266/3b5bf2be-bf24-486f-9970-0e31ffd5aba2)

![RockAnim](https://github.com/Novigra/UnrealEngineProject/assets/59344266/7b6c05eb-9c57-4952-8115-de597293049b)

Locomotion
![AnimBP](https://github.com/Novigra/UnrealEngineProject/assets/59344266/46f25a6c-c734-4465-96a7-2bf68cd96f18)

Don’t forget to implement the transitions
![AnimBP_Rock](https://github.com/Novigra/UnrealEngineProject/assets/59344266/6b2a96da-a2bd-4a36-9e3c-8aa99ef9239e)
---
Of course we don’t forget about the enemy, the implementation is pretty much the same with, of course, some slight changes.

EnemyBP
![Enemy_BP](https://github.com/Novigra/UnrealEngineProject/assets/59344266/292c2ae1-c013-473f-823b-6d4f0392d889)

Enemy Locomotion
![EnemyAnimBP](https://github.com/Novigra/UnrealEngineProject/assets/59344266/ff979c4b-1d54-4dfe-85e5-21f2d455523b)

In animation, you use more tools like Blend Space to switch between animations smoothly, we can also use it with aim offset to get the perfect animation, but my animations are so limited that I couldn’t implement them properly.

After the rock, paper, scissors match, both player and the enemy get pulled back so they can get ready for the next match.
![RPSMatch](https://github.com/Novigra/UnrealEngineProject/assets/59344266/3c9780a7-bee8-48fd-a639-be23da119f50)

To achieve this, I implemented constant interpolation, where the player interpolates to the box at a constant speed.
```
FVector CurrentLocation = GetActorLocation();
FVector TargetLocation = Placement->GetPlacementLocation();
FVector InterpLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
SetActorLocation(InterpLocation);
```

![InterpBlock](https://github.com/Novigra/UnrealEngineProject/assets/59344266/bb74a3fe-536f-48c9-9848-5df43a359eff)


---

# The Fight Match

---

Now that we have transitioned to the next phase, it’s time to talk about the weapons. Before I worked on the rifle weapon and the shotgun, I created a weapon class, which is the base class or parent class, to start with rules that both the rifle class and the shotgun class will inherit, and both of them will have their own rules. basic object oriented design.

![Weapons](https://github.com/Novigra/UnrealEngineProject/assets/59344266/a09bcb2b-1b3c-4b99-a33b-a719daf646bf)


When the player reaches the box player’s status transition from "pending" to "fight", then equip the weapon to achieve such a thing. You can use delegates, which is an awesome concept. The idea behind delegates is that when you want to implement an action like equipping a gun or managing the inventory system, it makes no sense to involve the player class, so you can add listeners so each class can do their own tasks and not rely on each other.



When setting the weapon, I attached it to a socket I created so it sticks to the hand rig bone.

```
if (PlayerWeapon == EPlayerWeapon::EPW_Shotgun)
		{
			MyPlayer->SetEquippedWeapon(this);

			const USkeletalMeshSocket* LeftHandSocket = MyPlayer->GetMesh()->GetSocketByName("L_palmSocket");
			if (LeftHandSocket)
			{
				LeftHandSocket->AttachActor(this, MyPlayer->GetMesh());
			}
		}
		else
		{
			Enemy->SetEnemyWeapon(this);

			const USkeletalMeshSocket* LeftHandSocket = Enemy->GetMesh()->GetSocketByName("LeftHandSocket");
			if (LeftHandSocket)
			{
				LeftHandSocket->AttachActor(this, Enemy->GetMesh());
			}
		}
```

## Shooting Implementation

---

To implement shooting, there are so many ways, and it depends on the game design and what makes more sense. The way I implemented it first, I needed both the camera vector and the weapon vector, specifically the arrow component, so I could get the new vector.

![VectorsEdit](https://github.com/Novigra/UnrealEngineProject/assets/59344266/fbf33f15-8c97-4bff-9d71-9585d485c817)

I used line trace to get the end vector as it provides helpful tools that you can use like HitResult, which provides details like the end vector, the actor that got hit, and much more.

Each weapon has a different implementation of how to handle bullets. The rifle weapon spawns one bullet every couple of seconds (it depends on the fire rate), but for the shotgun, it spawns four bullets at the same time, so I needed TArray and iterations.


Rifle Weapon

![Rifle](https://github.com/Novigra/UnrealEngineProject/assets/59344266/0772123b-31c7-4d43-a122-2cf020ce40e2)


Shotgun Weapon

![Shotgun](https://github.com/Novigra/UnrealEngineProject/assets/59344266/cd09c014-c0ef-4cdd-9acd-5f95631d1a62)


For the shotgun, I also added four arrow components so I could modify how each bullet moved.

![ShotgunWeapon](https://github.com/Novigra/UnrealEngineProject/assets/59344266/b87e46f2-88b6-4e47-ba89-5a6f5b2f8b4c)


![BulletRotation](https://github.com/Novigra/UnrealEngineProject/assets/59344266/e62726a1-581e-4f0b-8896-259f0daadd9b)



## Character Movement

---

To make the game more fair, the winner moves slower than the other player, but each one of them has their own quirks. The losing party can move faster and dash.

![Dash](https://github.com/Novigra/UnrealEngineProject/assets/59344266/89a5e0c6-f19c-4dad-9b48-814b5f28d7da)


But the winner can use a super punch that can launch them, but it can also be super deadly against the other player if they get hit.

![SuperPunch](https://github.com/Novigra/UnrealEngineProject/assets/59344266/a609746f-a8b4-4010-985b-75006ee36749)


That’s why I added pillars, the winner will be slow but can use the environment to their advantage.

![Pillars](https://github.com/Novigra/UnrealEngineProject/assets/59344266/a808d678-8377-4392-bc51-a84e7a053e65)

---

There’s more that needs to be done, like AI and making the game more responsive and interesting, but I'm currently focusing on the building blocks. 😉
