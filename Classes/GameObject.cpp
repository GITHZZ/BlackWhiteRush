//
//  GameObject.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-28.
//
//

#include "GameObject.h"

bool GameObject::init(){
    
    isActive = false;
    
    _objType = Object_Role;
    _ptmRadio = 32.0f;
    _bType = b2_staticBody;
    _groundIndex = 1;
    _density = 1.0f;
    _isTrigger = false;
    _friction = 1.0f;
    _speed = 10;
    _boxRect = CCSizeMake(0.0f, 0.0f);
    
    return true;
}

void GameObject::checkCollision(GameObject *collision){
    //if (!collision->getIsActive()) return;
    
    //获取碰撞体包围盒
    float curScale = this->getScale() - 0.5;
    CCRect objRect = CCRectMake(this->getBodyPosition().x - this->getContentSize().width/2 * curScale,
                                this->getBodyPosition().y - this->getContentSize().height/2 * curScale,
                                this->getContentSize().width * curScale,
                                this->getContentSize().height * curScale);
    float colliderCurScale = this->getScale() - 0.5;
    CCRect collider = CCRectMake(collision->getBodyPosition().x - collision->getContentSize().width/2 * colliderCurScale,
                                 collision->getBodyPosition().y - collision->getContentSize().height/2 * colliderCurScale,
                                 collision->getContentSize().width * colliderCurScale,
                                 collision->getContentSize().height * colliderCurScale);
    if (objRect.intersectsRect(collider)) {
        this->onCollisionEnter(collision);
    }
}

void GameObject::setB2Body(b2Body *body){
    _pB2Body = body;
}

b2Body* GameObject::getB2Body() const{
    return _pB2Body;
}

void GameObject::setPtmRadio(float ptm){
    _ptmRadio = ptm;
}

float GameObject::getPtmRadio() const{
    return _ptmRadio;
}

void GameObject::setBodyPosition(const cocos2d::CCPoint &position){
    if (!_pB2Body) return;
    float angle = _pB2Body->GetAngle();
    _pB2Body->SetTransform(b2Vec2(position.x/_ptmRadio,position.y/_ptmRadio),
                           angle);
}

CCPoint GameObject::getBodyPosition() const{
    //if (!_pB2Body) return this->getPosition();
    return ccp(_pB2Body->GetPosition().x * _ptmRadio,_pB2Body->GetPosition().y * _ptmRadio);
}

void GameObject::setBodyRotation(float fRotation){
    b2Vec2 p = _pB2Body->GetPosition();
    float rad = CC_DEGREES_TO_RADIANS(fRotation);
    _pB2Body->SetTransform(p, rad);
}

float GameObject::getBodyRotation() const{
    return CC_RADIANS_TO_DEGREES(_pB2Body->GetAngle());
}

CCAffineTransform GameObject::nodeToParentTransform(){
    b2Vec2 pos = _pB2Body->GetPosition();
    
    float x = pos.x * _ptmRadio;
    float y = pos.y * _ptmRadio;
    
    if (m_bIgnoreAnchorPointForPosition) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }
    
    float radians = _pB2Body->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);
    
    if (!m_obAnchorPointInPoints.equals(CCPointZero))
    {
		x += ((c * -m_obAnchorPointInPoints.x * m_fScaleX) + (-s * -m_obAnchorPointInPoints.y * m_fScaleY));
		y += ((s * -m_obAnchorPointInPoints.x * m_fScaleX) + (c * -m_obAnchorPointInPoints.y * m_fScaleY));
	}
    
    m_sTransform = CCAffineTransformMake( c * m_fScaleX,	s * m_fScaleX,
									     -s * m_fScaleY,	c * m_fScaleY,
									     x,	y );
	
	return m_sTransform;
}