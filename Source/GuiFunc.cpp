/*
  ==============================================================================

    GuiFunc.cpp
    Created: 24 Dec 2018 12:44:39pm
    Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GuiFunc.h"

//==============================================================================
volumeBox::volumeBox()
{
	volume = new Slider();

	addAndMakeVisible(volume);

	volume->setSliderStyle(Slider::LinearHorizontal);
	volume->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	volume->setRange(0.0f, 100.0f, 1.0);
}
volumeBox::~volumeBox(){}

void volumeBox::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);
	g.setColour(Colours::orange);
	g.drawRect(getLocalBounds(), 1);
	Path tri1, tri2, squ;
	float h = getHeight() * .25f;
	
	tri1.addTriangle(h, h, h, getHeight() - h, 3*h, getHeight()/2);
	tri2.addTriangle(getWidth() - h, h, getWidth() - h, getHeight() - h, getWidth() - (3 * h), getHeight() / 2);
	squ.addRectangle(getWidth() - (7 / 2 * h), (getHeight() / 2) - (h / 2), h, h);

	g.fillPath(tri1);
	g.fillPath(tri2);
	g.fillPath(squ);

	volume->setColour(Slider::thumbColourId, Colours::orange);
	volume->setColour(Slider::trackColourId, Colours::orange);
	volume->addListener(sListen);
	volume->setBounds(3.5f * h, 0.0f, getWidth() - (h*6), getHeight());

}

void volumeBox::resized()
{
}

void volumeBox::setListen(Slider::Listener* _sListen)
{
	sListen = _sListen;
}


//==============================================================================
GuiFunc::GuiFunc()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

}

GuiFunc::~GuiFunc()
{
}

void GuiFunc::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/


	//g.setColour(Colours::black);
	//auto area = getLocalBounds();
	//area.removeFromLeft((2 / 3.0 *(getHeight() *.9f)) + (getHeight() * .1f));
	//area.removeFromBottom(getHeight()- (getHeight() - area.getWidth()));
	//Path circle;
	//circle.addEllipse(area.getCentreX(), area.getCentreY(), area.getWidth()/2, area.getWidth()/2);
	//g.setColour(Colours::red);
	//g.fillPath(circle);

}

void GuiFunc::resized()
{
	emptyList();
	auto area = getLocalBounds();
	float yMargin = getHeight() * .1f;
	auto buttonArea = area.removeFromTop((getHeight()*2)/3);
	auto playArea = buttonArea.removeFromRight(getWidth()/3);
	
	buttonArea.reduce(2*yMargin, 0.0f);
	float h = buttonArea.getHeight() / 3;

	auto * btn1 = addToList(new TextButton("Panic"));
	btn1->setClickingTogglesState(false);
	btn1->setColour(TextButton::textColourOffId, Colours::black);
	btn1->setColour(TextButton::textColourOnId, Colours::white);
	btn1->setColour(TextButton::buttonColourId, Colours::red);
	btn1->setColour(TextButton::buttonOnColourId, Colours::orange);
	btn1->setBounds(buttonArea.removeFromTop(h));
	buttonArea.removeFromTop(h/2);

	auto * btn2 = addToList(new TextButton("Export"));
	btn2->setColour(TextButton::textColourOffId, Colours::white);
	btn2->setColour(TextButton::textColourOnId, Colours::black);
	btn2->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	btn2->setColour(TextButton::buttonOnColourId, Colours::orange);
	btn2->setBounds(buttonArea.removeFromTop(h));

	playArea.removeFromBottom(buttonArea.getHeight());
	playArea.reduce(buttonArea.getHeight() / 2, 0.0f);

	DrawablePath Normal, Over, Clicked;
	Path play;
	play.addEllipse(0.0f, 0.0f, playArea.getWidth(), playArea.getHeight());
	//play.addEllipse(playArea.getWidth()/2,playArea.getHeight()/2,playArea.getWidth()/2, playArea.getHeight()/2);
	Normal.setPath(play);
	Normal.setFill(Colours::orange);
	Normal.setStrokeFill(Colours::black);
	Normal.setStrokeThickness(.5f);

	Over.setPath(play);
	Over.setFill(Colours::darkorange);
	Over.setStrokeFill(Colours::black);
	Over.setStrokeThickness(.50f);

	Clicked.setPath(play);
	Clicked.setFill(Colours::red);
	Clicked.setStrokeFill(Colours::black);
	Clicked.setStrokeThickness(.50f);


	auto * btn3 = addToList(new DrawableButton("Play", DrawableButton::ImageRaw));
	btn3->setImages(&Normal, &Over, &Clicked);
	btn3->setBounds(playArea);
	btn3->setClickingTogglesState(false);

	auto * vBox = addToList(new volumeBox());
	vBox->setBounds(area.removeFromTop(h));
}


fundFreq::fundFreq()
{
	num = 0;
	note = &num;


}

fundFreq::~fundFreq()
{
}

void fundFreq::paint(Graphics & g)
{
	Colour bgColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	g.fillAll(bgColour);   // clear the background
	
	g.setColour(Colours::orange);
	g.drawRect(getLocalBounds(), 1);
	Point<float> p1(getWidth()*7.0f/12.0f, 0.0f);
	Point<float> p2(getWidth()*7.0f/12.0f,getBottom());
	
	Line<float> l(p1, p2);
	Point<float> ph1(getWidth()*7.0f / 12.0f, getHeight() / 2.0f);
	Point<float> ph2(getWidth(), getHeight() / 2.0f);
	Line<float> l1(ph1, ph2);
	g.drawLine(l, 1.0f);
	g.drawLine(l1, 1.0f);

}

void fundFreq::resized()
{
	emptyList();

	
	txtBox = new Label("fundFreq", "A");
	auto* tb = addToList(txtBox);
	tb->setBounds(0.0f,0.0f,getWidth() * 7/12,getHeight());
	tb->setJustificationType(Justification::centred);
	DrawablePath upNormal, downNormal, upOver, downOver, upClicked, downClicked;
	Path upPath, downPath;
	
	auto downArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, getHeight()/2.0f, getWidth() * 5/12, getHeight()/2);
	auto upArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, 0.0f, getWidth() * 5/12, getHeight() / 2);
	float xMargin = upArea.getWidth() * .2f;
	float yMargin = upArea.getHeight() *.2f;
	upPath.addTriangle(xMargin, upArea.getHeight() - yMargin, upArea.getWidth()/2, yMargin, upArea.getWidth() - xMargin, upArea.getHeight() - yMargin);
	downPath.addTriangle(xMargin,yMargin,downArea.getWidth() - xMargin, yMargin, downArea.getWidth()/2, downArea.getHeight() - yMargin);


	upNormal.setPath(upPath);
	upNormal.setFill(Colours::orange);
	upNormal.setStrokeFill(Colours::black);
	upNormal.setStrokeThickness(.5f);

	upOver.setPath(upPath);
	upOver.setFill(Colours::darkorange);
	upOver.setStrokeFill(Colours::black);
	upOver.setStrokeThickness(.50f);

	upClicked.setPath(upPath);
	upClicked.setFill(Colours::red);
	upClicked.setStrokeFill(Colours::black);
	upClicked.setStrokeThickness(.50f);

	downNormal.setPath(downPath);
	downNormal.setFill(Colours::orange);
	downNormal.setStrokeFill(Colours::black);
	downNormal.setStrokeThickness(.50f);

	downOver.setPath(downPath);
	downOver.setFill(Colours::red);
	downOver.setStrokeFill(Colours::black);
	downOver.setStrokeThickness(.50f);

	downClicked.setPath(downPath);
	downClicked.setFill(Colours::darkorange);
	downClicked.setStrokeFill(Colours::black);
	downClicked.setStrokeThickness(.50f);

	DrawableButton* upBtn = new DrawableButton("up", DrawableButton::ImageRaw);
	auto * btn = addToList(upBtn);
	btn->setImages(&upNormal, &upOver, &upClicked);
	btn->setBounds(upArea);
	btn->setClickingTogglesState(false);
	btn->onClick = [this] {
		num = (num++) % 12;
		updateText();
	};

	DrawableButton* downBtn = new DrawableButton("down", DrawableButton::ImageRaw);
	auto * btn2 = addToList(downBtn);
	btn2->setImages(&downNormal, &downOver, &downClicked);
	btn2->setBounds(downArea);
	btn2->setClickingTogglesState(false);
	btn2->onClick = [this] {
		if (num <= 0)
		{
			num = 11;
		}
		else
		{
			num = (num--) % 12;
		}
		updateText();
	};


}

void fundFreq::updateText()
{
	switch (num)
	{
		case 0:
			txtBox->setText("A", dontSendNotification);
			break;
		case 1:
			txtBox->setText("A#", dontSendNotification);
			break;
		case 2:
			txtBox->setText("B", dontSendNotification);
			break;
		case 3:
			txtBox->setText("C", dontSendNotification);
			break;
		case 4:
			txtBox->setText("C#", dontSendNotification);
			break;
		case 5:
			txtBox->setText("D", dontSendNotification);
			break;
		case 6:
			txtBox->setText("D#", dontSendNotification);
			break;
		case 7:
			txtBox->setText("E", dontSendNotification);
			break;
		case 8:
			txtBox->setText("F", dontSendNotification);
			break;
		case 9:
			txtBox->setText("F#", dontSendNotification);
			break;
		case 10:
			txtBox->setText("G", dontSendNotification);
			break;
		default:
			txtBox->setText("G#", dontSendNotification);
			break;

	}

}


lowMidQuadComp::lowMidQuadComp()
{
	addAndMakeVisible(cb);
	cb.addItem("fab", 1);
	cb.addItem("groovy", 2);
	cb.addItem("hep", 3);
	cb.addItem("mad for it", 4);
	cb.addItem("neat", 5);
	cb.addItem("swingin", 6);
	cb.addItem("wild", 7);
	cb.setSelectedItemIndex(3);
}

lowMidQuadComp::~lowMidQuadComp()
{
}

void lowMidQuadComp::paint(Graphics &)
{

}

void lowMidQuadComp::resized()
{ 
	emptyList();
	auto area = getLocalBounds();
	area.removeFromTop(150);
	float h = (area.getHeight()) / 3.0f;

	addAndMakeVisible(cb);
	//auto * test = addToList(&cb);		
	//cb->addItem("fab", 1);
	//cb->addItem("groovy", 2);
	//cb->addItem("hep", 3);
	//cb->addItem("mad for it", 4);
	//cb->addItem("neat", 5);
	//cb->addItem("swingin", 6);
	//cb->addItem("wild", 7);

	//comboBox->onChange = [this] { owner.setRating(row, comboBox.getSelectedId()); };
	//combobox->setWantsKeyboardFocus(false);
	//combobox->onChange = [this] { setAllLookAndFeels(lookAndFeels[lafBox.getSelectedItemIndex()]); };
	
	cb.setBounds(area.removeFromTop(22).removeFromLeft(250));
}
