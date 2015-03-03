import bb.cascades 1.3
import bb.system 1.2
import player 1.0

SystemProgressToast {
    modality: SystemUiModality.Application
    state: SystemUiProgressState.Active
    position: SystemUiPosition.MiddleCenter
    statusMessage: ""
    button.label: qsTr("Hide") + Retranslate.onLanguageChanged
    button.enabled: false
}