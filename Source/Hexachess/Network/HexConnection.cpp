#include "HexConnection.h"

#include "SocketSubsystem.h"
#include "HttpModule.h"
#include "Http.h"

void AHexConnection::GetPublicIPAddress()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHexConnection::OnIPAddressResponseReceived);
	Request->SetURL(TEXT("https://httpbin.org/ip")); // Replace with the appropriate API endpoint
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void AHexConnection::OnIPAddressResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		FString ResponseStr = Response->GetContentAsString();
		// Parse the JSON response to extract the public IP address.
		// The JSON structure may vary depending on the API used.
		// In the case of httpbin.org, it returns something like: {"origin": "x.x.x.x"}
        
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString PublicIPAddress = JsonObject->GetStringField("origin");
			MyIP = PublicIPAddress;
			UE_LOG(LogTemp, Warning, TEXT("Public IP Address: %s"), *PublicIPAddress);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve public IP address."));
	}
}

FString AHexConnection::GetIP()
{
	return MyIP;
}

FString AHexConnection::IpToHex(const FString& Ip, const FString& Key)
{
	TArray<FString> Octets;
	Ip.ParseIntoArray(Octets, TEXT("."), true);
    
	FString HexValue;
	for (int32 i = 0; i < Octets.Num(); ++i)
	{
		int32 Octet = FCString::Atoi(*Octets[i]);
		int32 KeyByte = Key.IsValidIndex(i) ? (uint8)Key[i] : 0;
		HexValue += FString::Printf(TEXT("%02X"), Octet ^ KeyByte);
	}

	return HexValue;
}

FString AHexConnection::HexToIp(const FString& HexValue, const FString& Key)
{
	FString Ip;
	for (int32 i = 0; i < HexValue.Len(); i += 2)
	{
		FString HexOctet = HexValue.Mid(i, 2);
		int32 Octet;
		Octet = FCString::Strtoi(*HexOctet, nullptr, 16);
        
		int32 KeyByte = Key.IsValidIndex(i / 2) ? (uint8)Key[i / 2] : 0;
		Ip += FString::Printf(TEXT("%d"), Octet ^ KeyByte);
		if (i < 6)
		{
			Ip += TEXT(".");
		}
	}

	return Ip;
}

FString AHexConnection::ConvertIpWithUTCKey(const FString& Ip)
{
	FDateTime Now = FDateTime::UtcNow();
	uint8 UTCSeconds = Now.GetSecond();
    
	FString Key = FString::Printf(TEXT("%02X"), UTCSeconds);
    
	// Передаем ключ для XOR-шифрования IP
	FString EncodedIpPart = IpToHex(Ip, Key);
    
	return Key + EncodedIpPart;
}

FString AHexConnection::RetrieveIpWithUTCKey(const FString& EncodedIp)
{
	if (EncodedIp.Len() != 10) return "Invalid Input";

	FString Key = EncodedIp.Left(2);
	FString HexValue = EncodedIp.Right(8);
    
	return HexToIp(HexValue, Key);
}