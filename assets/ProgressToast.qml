import bb.cascades 1.0
import bb.system 1.0
import player 1.0

SystemProgressToast {
    modality: SystemUiModality.Application
    state: SystemUiProgressState.Active
    position: SystemUiPosition.MiddleCenter
    statusMessage: ""
    button.label: qsTr("Hide") + Retranslate.onLanguageChanged
    button.enabled: false
}