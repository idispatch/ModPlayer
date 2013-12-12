import bb.cascades 1.0
import bb.system 1.0

SystemPrompt {
    title: qsTr("Enter Playlist Name")
    modality: SystemUiModality.Application
    dismissAutomatically: true
    includeRememberMe: false
    inputField.emptyText: qsTr("Playlist Name")
    inputField.defaultText: "Playlist"
    inputField.inputMode: SystemUiInputMode.Default
    inputField.maximumLength: 20
    function run() {
        exec()
        if(result != SystemUiResult.ConfirmButtonSelection)
            return ""
        return inputFieldTextEntry()
    }
}