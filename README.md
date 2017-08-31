
like iOS UIPickerView


Pure c++ code

//create
	pSpinner = nullptr;
    
    setSpinner(DXSpinner::spinner());//create
    
    pSpinner->setBackGroundColor(cocos2d::Color3B::BLUE);
    pSpinner->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    
    pSpinner->setDelegate(this); //set delegate
    pSpinner->setDataSource(this); //set datasource
    pSpinner->setContentSize(cocos2d::Size(400, 200));//set size before  add to parent view
    
    this->addChild(pSpinner); //add to view, will reload All Components

    pSpinner->setPosition(cocos2d::Vec2(this->getContentSize().width/6,this->getContentSize().height/4));
    
    pSpinner->selectRow(3, 1, true);//select row in Component



like ios UIPickerView 's Delegate
//Delegate
struct DXSpinnerDelegate : ProtocolBase {
    virtual const std::string titleForRowInComponent(DXSpinner *spinner, ssize_t row, ssize_t inComponent) = 0; //return title for row in component
    
    virtual void didSelectRowInComponent(DXSpinner *spinner, ssize_t row, ssize_t inComponent) {}//callback when selected row changed 
};


like ios UIPickerView 's DataSource
//DataSource
struct DXSpinnerDataSource : ProtocolBase {
    virtual ssize_t numberOfComponents(DXSpinner *spinner) {return 1;};//Specifies the number of components
    virtual ssize_t numberOfRowsInComponent(DXSpinner *spinner, ssize_t inComponent) = 0;//Returns the number of rows for the specified component
};



![image](https://raw.githubusercontent.com/doorxp/DXSpinnerDemo/master/Screen%20Shot%202017-08-31%20at%20%E4%B8%8A%E5%8D%889.46.27.png)