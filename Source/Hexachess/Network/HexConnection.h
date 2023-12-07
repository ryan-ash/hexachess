#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

#include "HexConnection.generated.h"


UCLASS()
class AHexConnection : public AActor
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FString GetIP();

    UFUNCTION(BlueprintCallable, Category = "IP Conversion")
    static FString ConvertIpWithUTCKey(const FString& Ip, bool UseStaticKey);
    
    UFUNCTION(BlueprintCallable, Category = "IP Conversion")
    static FString RetrieveIpWithUTCKey(const FString& EncodedIp);

	UFUNCTION(BlueprintCallable, Category = "IP Conversion")
	static FString IpToHex(const FString& Ip, const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "IP Conversion")
	static FString HexToIp(const FString& HexValue, const FString& Key);

	UFUNCTION(BlueprintCallable)
	void GetPublicIPAddress();

	UFUNCTION(BlueprintCallable)
	FString GetMyIP() { return MyIP; }
	
private:
	
	void OnIPAddressResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	FString MyIP;
};