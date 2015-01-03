import bb.cascades 1.0
import bb.system 1.0

SystemToast {
    id: buyResult
    modality: SystemUiModality.Application
    position: SystemUiPosition.MiddleCenter
    body: qsTr("This feature is available in ModPlayer Plus.") + Retranslate.onLanguageChanged
    button.label: "Ok"
}