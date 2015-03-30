import bb.cascades 1.0

Container {
    id: indexLettersView
    property variant dataModel
    property int minimalItemCount: 5
    signal itemSelected(string item)
    horizontalAlignment: HorizontalAlignment.Right
    verticalAlignment: VerticalAlignment.Center
    onCreationCompleted: {
        dataModelChanged.connect(createLetters)
        createLetters()
    }
    function letterFocused(letter) {
        itemSelected(letter)
    }
    function createLetters() {
        indexLettersContainer.removeAll()
        if(dataModel != null && dataModel.size() > minimalItemCount) {
            var count = dataModel.childCount([])
            for(var i = 0; i < count; ++i) {
                var letter = dataModel.data([i])
                if(count <= 26 || (letter >= 'A' && letter <= 'Z')) {
                    var letterObject = letterView.createObject()
                    if(letterObject != null) {
                        letterObject.text = letter 
                        letterObject.focused.connect(letterFocused)
                        indexLettersContainer.add(letterObject)
                    }
                }
            }
        } 
    }
    attachedObjects: [
        ComponentDefinition {
            id: letterView
            Container {
                id: letterViewRoot
                signal focused(string letter)
                property string text
                property bool selected: false
                preferredWidth: selected ? 80 : 50
                horizontalAlignment: HorizontalAlignment.Center
                background: letterViewBackgroundPaint.imagePaint
                leftPadding: 13
                bottomPadding: 4
                bottomMargin: 1
                topMargin: 1
                attachedObjects: [
                    ImagePaintDefinition {
                        id: letterViewBackgroundPaint
                        imageSource: "asset:///images/backgrounds/indicator2.png"
                    }
                ]
                Label {
                    id: letter
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    text: letterViewRoot.text
                    textStyle.fontSize: letterViewRoot.selected ? FontSize.Medium : FontSize.XXSmall
                    textStyle.fontWeight: letterViewRoot.selected ? FontWeight.W900 : FontWeight.W500
                    textStyle.color: Color.White
                }
                onTouchEnter: {
                    letterViewRoot.selected = true
                    focused(letterViewRoot.text)
                }
                onTouchExit: {
                    letterViewRoot.selected = false
                }
                onTouch: {
                    if(event.isDown() || event.isMove()) {
                        letterViewRoot.selected = true
                        focused(letterViewRoot.text)
                    } else {
                        letterViewRoot.selected = false
                    }
                }
            }
        }
    ]
    Container {
        id: indexLettersContainer
        horizontalAlignment: HorizontalAlignment.Right
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        background: Color.Transparent
        minWidth: 50
    }
}