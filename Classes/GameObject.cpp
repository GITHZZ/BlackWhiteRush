//
//  GameObject.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-28.
//
//

#include "GameObject.h"

bool GameObject::inits(){
    
    isActive = false;
    
    _objType = Object_Role;
    _ptmRadio = 32.0f;
    _bType = b2_staticBody;
    _groundIndex = 1;
    _density = 1.0f;
    _isTrigger = false;
    _friction = 1.0f;
    _speed = 10;
    _isBox2DBody = false;
    _boxRect = CCSizeMake(0.0f, 0.0f);

    return true;
}

void GameObject::checkCollision(GameObject *collision){
    //if (!collision->getIsActive()) return;
    
    //获取碰撞体包围盒
    float curScale = this->getScale() - 1.0f;
    CCRect objRect = CCRectMake(this->getObjPosition().x - this->getContentSize().width/2 * curScale,
                                this->getObjPosition().y - this->getContentSize().height/2 * curScale,
                                this->getContentSize().width * curScale,
                                this->getContentSize().height * curScale);
    float colliderCurScale = this->getScale() - 1.0f;
    CCRect collider = CCRectMake(collision->getObjPosition().x - collision->getContentSize().width/2 * colliderCurScale,
                                 collision->getObjPosition().y - collision->getContentSize().height/2 * colliderCurScale,
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

void GameObject::setObjPosition(const cocos2d::CCPoint &position){
    if (_isBox2DBody) {
        if (!_pB2Body) return;
        float angle = _pB2Body->GetAngle();
        _pB2Body->SetTransform(b2Vec2(position.x/_ptmRadio,position.y/_ptmRadio),
                               angle);
    }else{
        this->setPosition(position);
    }
}

CCPoint GameObject::getObjPosition(){
    if (_isBox2DBody)
        return ccp(_pB2Body->GetPosition().x * _ptmRadio,_pB2Body->GetPosition().y * _ptmRadio);
    else
        return this->getPosition();
}

void GameObject::setObjRotation(float fRotation){
    if (_isBox2DBody) {
        b2Vec2 p = _pB2Body->GetPosition();
        float rad = CC_DEGREES_TO_RADIANS(fRotation);
        _pB2Body->SetTransform(p, rad);
    }else{
        this->setRotation(fRotation);
    }
}

float GameObject::getObjRotation(){
    if (_isBox2DBody)
        return CC_RADIANS_TO_DEGREES(_pB2Body->GetAngle());
    else
        return this->getRotation();
}

CCAffineTransform GameObject::nodeToParentTransform(){
    if (_isBox2DBody) {
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
        
    }else{
        if (m_bTransformDirty)
        {
            
            // Translate values
            float x = m_obPosition.x;
            float y = m_obPosition.y;
            
            if (m_bIgnoreAnchorPointForPosition)
            {
                x += m_obAnchorPointInPoints.x;
                y += m_obAnchorPointInPoints.y;
            }
            
            // Rotation values
            // Change rotation code to handle X and Y
            // If we skew with the exact same value for both x and y then we're simply just rotating
            float cx = 1, sx = 0, cy = 1, sy = 0;
            if (m_fRotationX || m_fRotationY)
            {
                float radiansX = -CC_DEGREES_TO_RADIANS(m_fRotationX);
                float radiansY = -CC_DEGREES_TO_RADIANS(m_fRotationY);
                cx = cosf(radiansX);
                sx = sinf(radiansX);
                cy = cosf(radiansY);
                sy = sinf(radiansY);
            }
            
            bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );
            
            
            // optimization:
            // inline anchor point calculation if skew is not needed
            // Adjusted transform calculation for rotational skew
            if (! needsSkewMatrix && !m_obAnchorPointInPoints.equals(CCPointZero))
            {
                x += cy * -m_obAnchorPointInPoints.x * m_fScaleX + -sx * -m_obAnchorPointInPoints.y * m_fScaleY;
                y += sy * -m_obAnchorPointInPoints.x * m_fScaleX +  cx * -m_obAnchorPointInPoints.y * m_fScaleY;
            }
            
            
            // Build Transform Matrix
            // Adjusted transform calculation for rotational skew
            m_sTransform = CCAffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
                                                 -sx * m_fScaleY, cx * m_fScaleY,
                                                 x, y );
            
            // XXX: Try to inline skew
            // If skew is needed, apply skew and then anchor point
            if (needsSkewMatrix)
            {
                CCAffineTransform skewMatrix = CCAffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                                                                     tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
                                                                     0.0f, 0.0f );
                m_sTransform = CCAffineTransformConcat(skewMatrix, m_sTransform);
                
                // adjust anchor point
                if (!m_obAnchorPointInPoints.equals(CCPointZero))
                {
                    m_sTransform = CCAffineTransformTranslate(m_sTransform, -m_obAnchorPointInPoints.x, -m_obAnchorPointInPoints.y);
                }
            }
            
            if (m_bAdditionalTransformDirty)
            {
                m_sTransform = CCAffineTransformConcat(m_sTransform, m_sAdditionalTransform);
                m_bAdditionalTransformDirty = false;
            }
            
            m_bTransformDirty = false;
        }
    }
    
    return m_sTransform;
}