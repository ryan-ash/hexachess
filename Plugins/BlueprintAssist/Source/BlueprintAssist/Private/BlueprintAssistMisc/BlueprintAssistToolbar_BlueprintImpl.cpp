#include "BlueprintAssistMisc/BlueprintAssistToolbar_BlueprintImpl.h"

#include "BlueprintAssistGraphHandler.h"
#include "BlueprintAssistUtils.h"
#include "EdGraphNode_Comment.h"
#include "MessageLogModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"

void FBAToolbar_BlueprintImpl::DetectUnusedNodes()
{
	struct FLocal
	{
		static void DeleteNode(const TSharedRef<IMessageToken>& Token)
		{
			const TSharedRef<FUObjectToken> UObjectToken = StaticCastSharedRef<FUObjectToken>(Token);
			if (UObjectToken->GetObject().IsValid())
			{
				if (UEdGraphNode* Node = Cast<UEdGraphNode>(UObjectToken->GetObject().Get()))
				{
					FScopedTransaction Transaction(INVTEXT("Delete Unused Node"));
					FBAUtils::DeleteNode(Node);
				}
			}
		}

		static void JumpToNode(const TSharedRef<IMessageToken>& Token)
		{
			const TSharedRef<FUObjectToken> UObjectToken = StaticCastSharedRef<FUObjectToken>(Token);
			if (UObjectToken->GetObject().IsValid())
			{
				if (UEdGraphNode* Node = Cast<UEdGraphNode>(UObjectToken->GetObject().Get()))
				{
					TSharedPtr<FBAGraphHandler> GraphHandler = FBAUtils::GetCurrentGraphHandler();
					if (GraphHandler)
					{
						TSharedPtr<SGraphEditor> GraphEditor = GraphHandler->GetGraphEditor();
						if (GraphEditor.IsValid())
						{
							GraphEditor->JumpToNode(Node);
						}
					}
				}
			}
		}
	};
	
	if (TSharedPtr<FBAGraphHandler> GraphHandler = FBAUtils::GetCurrentGraphHandler())
	{
		UEdGraph* Graph = GraphHandler->GetFocusedEdGraph();

		if (!Graph || FBlueprintEditorUtils::IsGraphReadOnly(Graph))
		{
			return;
		}

		for (int32 i = Graph->Nodes.Num() - 1; i >= 0; --i)
		{
			UEdGraphNode* Node = Graph->Nodes[i];

			if (Node->CanUserDeleteNode() &&
				!Node->IsA<UEdGraphNode_Comment>() &&
				FBAUtils::GetLinkedPins(Node).FilterByPredicate(FBAUtils::IsExecPin).Num() == 0)
			{
				FMessageLog MessageLog("BlueprintAssist");
#if BA_UE_VERSION_OR_LATER(5, 1)
				MessageLog.SetCurrentPage(INVTEXT("Unused Nodes"));
#else
				MessageLog.NewPage(INVTEXT("Unused Nodes"));
#endif

				TSharedRef<FTokenizedMessage> Message = FTokenizedMessage::Create(EMessageSeverity::Info);
				Message->AddToken(FTextToken::Create(FText::Format(INVTEXT("Unused node {0}"), FText::FromString(FBAUtils::GetNodeName(Node)))));

				Message->AddToken(FUObjectToken::Create(Node, INVTEXT("Navigate"))
					->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(FLocal::JumpToNode)));

				Message->AddToken(FUObjectToken::Create(Node, INVTEXT("Delete"))
					->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(FLocal::DeleteNode)));

				MessageLog.AddMessage(Message);
				
				FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
				MessageLogModule.OpenMessageLog("BlueprintAssist");
			}
		}
	}
}
