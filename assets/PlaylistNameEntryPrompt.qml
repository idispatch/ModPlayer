import bb.cascades 1.0
import bb.system 1.0

SystemPrompt {
    title: qsTr("Enter Playlist Name") + Retranslate.onLanguageChanged
    modality: SystemUiModality.Application
    dismissAutomatically: true
    includeRememberMe: false
    inputField.emptyText: qsTr("Playlist Name") + Retranslate.onLanguageChanged
    inputField.defaultText: "Playlist"
    inputField.inputMode: SystemUiInputMode.Default
    inputField.maximumLength: 30
    function run() {
        exec()
        if(result != SystemUiResult.ConfirmButtonSelection)
            return ""
        return inputFieldTextEntry().trim()
    }
}