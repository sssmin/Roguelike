#include "RoguelikeLoadingScreen.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h" 
#include "Blueprint/UserWidget.h"
 
class FRoguelikeLoadingScreenModule : public IRoguelikeLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/T_MyProject_TransparentLogo.T_MyProject_TransparentLogo") );
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}
 
	virtual void StartInGameLoadingScreen(UUserWidget* Widget, bool bPlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
 
	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}
};
 
IMPLEMENT_GAME_MODULE(FRoguelikeLoadingScreenModule, RoguelikeLoadingScreen);